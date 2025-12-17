#ifndef RLE_COMPRESSOR_HPP
#define RLE_COMPRESSOR_HPP

#include "ICompressor.hpp"
#include "../Streams/BinaryStream.hpp"
#include <vector>
#include <chrono>

class RLECompressor final : public ICompressor<BinaryStream, BinaryStream> {
private:
    CompressionStats stats;
    static constexpr char MAX_RUN_LENGTH = 127;

public:
    RLECompressor() = default;
    ~RLECompressor() override = default;

    void Compress(BinaryStream& input, BinaryStream& output) override {
        ResetStats();
        const size_t originalSize = input.getSize();
        if (originalSize == 0) {
            stats = CompressionStats(0, 0, 0, GetAlgorithmName());
            return;
        }

        auto startTime = std::chrono::high_resolution_clock::now();
        input.Seek(0);
        std::vector<char> buffer(std::min(originalSize, static_cast<size_t>(1024 * 1024)));
        size_t totalRead = 0;

        while (totalRead < originalSize) {
            size_t toRead = std::min(buffer.size(), originalSize - totalRead);
            size_t bytesRead = input.ReadBlock(buffer.data(), toRead);
            if (bytesRead == 0) break;
            encodeRLE(buffer.data(), bytesRead, output);
            totalRead += bytesRead;
        }
        output.Flush();
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = endTime - startTime;
        stats = CompressionStats(originalSize, output.getSize(), duration.count(), GetAlgorithmName());
    }
    void Decompress(BinaryStream& input, BinaryStream& output) override {
        ResetStats();
        const size_t compressedSize = input.getSize();
        auto startTime = std::chrono::high_resolution_clock::now();

        if (compressedSize == 0) {
            stats = CompressionStats(0, 0, 0, GetAlgorithmName() + " (decompression)");
            return;
        }

        input.Seek(0);
        size_t totalProcessed = 0;
        while (totalProcessed < compressedSize) {
            if (compressedSize - totalProcessed < 2) throwError(RLE_ERROR);
            char count = input.ReadByte();
            char value = input.ReadByte();
            totalProcessed += 2;
            for (int i = 0; i < static_cast<unsigned char>(count); i++) {
                output.WriteByte(value);
            }
        }
        output.Flush();
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = endTime - startTime;
        stats = CompressionStats(compressedSize, output.getSize(), duration.count(), GetAlgorithmName() + " (decompression)");
    }

    [[nodiscard]] CompressionStats GetStats() const override { return stats; }
    [[nodiscard]] std::string GetAlgorithmName() const override { return "RLE Compression"; }
    void ResetStats() override { stats = CompressionStats(); }

private:
    static void encodeRLE(const char* data, size_t size, BinaryStream& output) {
        size_t i = 0;
        while (i < size) {
            char current = data[i];
            char count = 1;

            while (i + count < size && count < MAX_RUN_LENGTH && data[i + count] == current) {
                count++;
            }

            output.WriteByte(count);
            output.WriteByte(current);

            i += count;
        }
    }
};

#endif
