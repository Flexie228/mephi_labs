#ifndef LZW_COMPRESSOR_HPP
#define LZW_COMPRESSOR_HPP

#include "ICompressor.hpp"
#include "../Streams/BinaryStream.hpp"
#include <unordered_map>
#include <string>
#include <vector>
#include <cstdint>

class LZWCompressor final : public ICompressor<BinaryStream, BinaryStream> {
private:
    CompressionStats stats;

    static constexpr int CODE_BITS = 12;
    static constexpr int MAX_CODES = 1 << CODE_BITS;
    static constexpr uint16_t CLEAR_CODE = 256;
    static constexpr uint16_t END_OF_DATA = 257;
    static constexpr uint16_t FIRST_CODE = 258;

    class BitWriter {
    private:
        BinaryStream& stream;
        uint32_t buffer = 0;
        int bitsInBuffer = 0;

    public:
        explicit BitWriter(BinaryStream& output) : stream(output) {}

        void writeBits(uint16_t code, int bits) {
            buffer |= (static_cast<uint32_t>(code) << bitsInBuffer);
            bitsInBuffer += bits;

            while (bitsInBuffer >= 8) {
                char byte = static_cast<char>(buffer & 0xFF);
                stream.WriteByte(byte);
                buffer >>= 8;
                bitsInBuffer -= 8;
            }
        }

        // ReSharper disable once CppMemberFunctionMayBeConst
        void flush() {
            if (bitsInBuffer > 0) {
                char byte = static_cast<char>(buffer & 0xFF);
                stream.WriteByte(byte);
            }
        }

        ~BitWriter() { flush(); }
    };

    class BitReader {
    private:
        BinaryStream& stream;
        uint32_t buffer = 0;
        int bitsInBuffer = 0;
        bool eofReached = false;

    public:
        explicit BitReader(BinaryStream& input) : stream(input) {}

        uint16_t readBits(int bits) {
            while (bitsInBuffer < bits && !eofReached) {
                if (stream.isEOF()) {
                    eofReached = true;
                    break;
                }
                char byte = stream.ReadByte();
                buffer |= (static_cast<uint32_t>(static_cast<unsigned char>(byte)) << bitsInBuffer);
                bitsInBuffer += 8;
            }

            if (bitsInBuffer < bits) {
                throw std::runtime_error("Not enough bits to read");
            }

            auto result = static_cast<uint16_t>(buffer & ((1u << bits) - 1));
            buffer >>= bits;
            bitsInBuffer -= bits;
            return result;
        }

        [[nodiscard]] bool eof() const {
            return eofReached && bitsInBuffer == 0;
        }
    };

public:
    LZWCompressor() = default;
    ~LZWCompressor() override = default;

    void Compress(BinaryStream& input, BinaryStream& output) override {
        ResetStats();
        const size_t originalSize = input.getSize();

        if (originalSize == 0) {
            stats = CompressionStats(0, 0, 0, GetAlgorithmName());
            return;
        }
        auto startTime = std::chrono::high_resolution_clock::now();
        input.Seek(0);

        // Инициализация словаря
        std::unordered_map<std::string, uint16_t> dictionary;
        for (int i = 0; i < 256; i++) {
            dictionary[std::string(1, static_cast<char>(i))] = static_cast<uint16_t>(i);
        }

        BitWriter bitWriter(output);

        // Начинаем с CLEAR_CODE
        bitWriter.writeBits(CLEAR_CODE, CODE_BITS);

        std::string current;
        uint16_t nextCode = FIRST_CODE;
        size_t totalBytes = 0;

        while (!input.isEOF()) {
            char byte;
            try {
                byte = input.ReadByte();
                totalBytes++;
            } catch (...) {
                break;
            }

            std::string next = current + byte;

            if (dictionary.contains(next)) {
                current = next;
            } else {
                // Записываем код для current
                uint16_t code = dictionary[current];
                bitWriter.writeBits(code, CODE_BITS);

                // Добавляем next в словарь
                if (nextCode < MAX_CODES) {
                    dictionary[next] = nextCode++;
                }

                // Если словарь полон, сбрасываем
                if (nextCode == MAX_CODES) {
                    bitWriter.writeBits(CLEAR_CODE, CODE_BITS);

                    dictionary.clear();
                    for (int i = 0; i < 256; i++) {
                        dictionary[std::string(1, static_cast<char>(i))] = static_cast<uint16_t>(i);
                    }
                    nextCode = FIRST_CODE;
                }

                current = std::string(1, byte);
            }
        }

        // Записываем последний код
        if (!current.empty()) {
            uint16_t code = dictionary[current];
            bitWriter.writeBits(code, CODE_BITS);
        }

        // Конец данных
        bitWriter.writeBits(END_OF_DATA, CODE_BITS);
        bitWriter.flush();
        output.Flush();
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = endTime - startTime;
        stats = CompressionStats(originalSize, output.getSize(), duration.count(), GetAlgorithmName());
    }

    void Decompress(BinaryStream& input, BinaryStream& output) override {
        ResetStats();
        const size_t compressedSize = input.getSize();

        if (compressedSize == 0) {
            stats = CompressionStats(0, 0, 0, GetAlgorithmName() + " (decompression)");
            return;
        }
        auto startTime = std::chrono::high_resolution_clock::now();
        input.Seek(0);

        BitReader bitReader(input);

        // Инициализация словаря
        std::vector<std::string> dictionary(MAX_CODES);
        for (int i = 0; i < 256; i++) {
            dictionary[i] = std::string(1, static_cast<char>(i));
        }

        // Читаем первый код (должен быть CLEAR_CODE)
        uint16_t code = bitReader.readBits(CODE_BITS);
        if (code != CLEAR_CODE) {
            throwError(LZW_ERROR);
        }

        code = bitReader.readBits(CODE_BITS);
        if (code == END_OF_DATA) {
            // Пустой файл
            return;
        }

        // Первый символ после CLEAR_CODE
        if (code >= dictionary.size()) {
            throwError(LZW_ERROR);
        }

        std::string current = dictionary[code];
        output.WriteBlock(current.c_str(), current.size());

        std::string previous = current;
        uint16_t nextCode = FIRST_CODE;

        while (!bitReader.eof()) {
            code = bitReader.readBits(CODE_BITS);

            if (code == END_OF_DATA) {
                break;
            }

            if (code == CLEAR_CODE) {
                // Сброс словаря
                dictionary.clear();
                dictionary.resize(MAX_CODES);
                for (int i = 0; i < 256; i++) {
                    dictionary[i] = std::string(1, static_cast<char>(i));
                }
                nextCode = FIRST_CODE;

                // Читаем следующий код
                code = bitReader.readBits(CODE_BITS);
                if (code == END_OF_DATA) break;

                current = dictionary[code];
                output.WriteBlock(current.c_str(), current.size());
                previous = current;
                continue;
            }

            std::string entry;

            if (code < nextCode) {
                // Код уже в словаре
                entry = dictionary[code];
            } else if (code == nextCode) {
                // Специальный случай
                entry = previous + previous[0];
            } else {
                throwError(LZW_ERROR);
            }

            output.WriteBlock(entry.c_str(), entry.size());

            // Добавляем в словарь
            if (nextCode < MAX_CODES) {
                dictionary[nextCode] = previous + entry[0];
                nextCode++;
            }

            previous = entry;
        }
        output.Flush();
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = endTime - startTime;
        stats = CompressionStats(compressedSize, output.getSize(), duration.count(), GetAlgorithmName() + " decompression");
    }

    [[nodiscard]] CompressionStats GetStats() const override { return stats; }
    [[nodiscard]] std::string GetAlgorithmName() const override { return "LZW "; }
    void ResetStats() override { stats = CompressionStats(); }
};

#endif // LZW_COMPRESSOR_HPP