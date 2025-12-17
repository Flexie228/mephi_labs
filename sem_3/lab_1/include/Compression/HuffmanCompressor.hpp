#ifndef HUFFMAN_COMPRESSOR_HPP
#define HUFFMAN_COMPRESSOR_HPP

#include <functional>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <memory>
#include <cinttypes>
#include "ICompressor.hpp"
#include "../Streams/ReadStream.hpp"
#include "../Streams/WriteStream.hpp"
using namespace std;

class HuffmanCompressor final : public ICompressor<ReadStream, WriteStream> {
private:
    struct HuffmanNode {
        char character;
        int frequency;
        std::unique_ptr<HuffmanNode> left;
        std::unique_ptr<HuffmanNode> right;

        HuffmanNode(const char ch, const int freq) : character(ch), frequency(freq), left(nullptr), right(nullptr) {}

        HuffmanNode(const int freq, std::unique_ptr<HuffmanNode> l, std::unique_ptr<HuffmanNode> r)
            : character('\0'), frequency(freq), left(std::move(l)), right(std::move(r)) {}

        [[nodiscard]] bool isLeaf() const { return left == nullptr && right == nullptr; }
    };

    // Функтор для сравнения узлов в priority_queue (binary heap) Самые редкие сочетания символов -> вверх (самый длинный код)
    struct CompareNodes {
        bool operator()(const std::unique_ptr<HuffmanNode>& a, const std::unique_ptr<HuffmanNode>& b) const {
            return a->frequency > b->frequency;
        }
    };

    std::unique_ptr<HuffmanNode> root;
    std::unordered_map<char, std::string> huffmanCodes;
    std::unordered_map<std::string, char> reverseCodes;
    CompressionStats stats;

    void buildFrequencyTable(ReadStream& input) {
        std::unordered_map<char, int> frequencyTable;

        input.Seek(0);
        while (true) {
            try {
                if (input.isEOF()) break;
                char ch = input.ReadChar();
                ++frequencyTable[ch];
            } catch (const std::exception& e) {
                if (std::string(e.what()).find("EOF") != std::string::npos) {
                    break;
                }
                throw;
            }
        }

        buildHuffmanTree(frequencyTable);
    }
    void buildHuffmanTree(const std::unordered_map<char, int>& frequencyTable) {
        if (frequencyTable.empty()) {
            root = nullptr;
            return;
        }

        std::priority_queue<std::unique_ptr<HuffmanNode>, std::vector<std::unique_ptr<HuffmanNode>>, CompareNodes> pq;
        for (const auto& [character, frequency] : frequencyTable) {
            pq.push(std::make_unique<HuffmanNode>(character, frequency));
        }
        for (const auto& [ch, code] : huffmanCodes) {
            std::cout << "  '" << ch << "' -> " << code << " (length: " << code.length() << ")" << std::endl;
        }

        while (pq.size() > 1) {
            auto left = std::move(const_cast<std::unique_ptr<HuffmanNode>&>(pq.top()));
            pq.pop();
            auto right = std::move(const_cast<std::unique_ptr<HuffmanNode>&>(pq.top()));
            pq.pop();

            int parentFreq = left->frequency + right->frequency;
            auto parent = std::make_unique<HuffmanNode>(parentFreq, std::move(left), std::move(right));
            pq.push(std::move(parent));
        }
        root = std::move(const_cast<std::unique_ptr<HuffmanNode>&>(pq.top()));
        pq.pop();

        generateCodes(root, "");
    }
    void generateCodes(const std::unique_ptr<HuffmanNode>& node, const std::string& code) {
        if (node == nullptr) return;

        if (node->isLeaf()) {
            // ЕСЛИ code пустая - делаем ее "0"
            std::string finalCode = code.empty() ? "0" : code;
            huffmanCodes[node->character] = finalCode;
            reverseCodes[finalCode] = node->character;
        } else {
            generateCodes(node->left, code + "0");
            generateCodes(node->right, code + "1");
        }
    }
    void writeTree(WriteStream& output) const {
        if (root == nullptr) return;


        function<void(const std::unique_ptr<HuffmanNode>&, WriteStream&)> writeNode = [&](const std::unique_ptr<HuffmanNode>& node, WriteStream& out) {
            if (node->isLeaf()) {
                out.WriteChar('1');
                out.WriteChar(node->character);
            } else {
                out.WriteChar('0');
                writeNode(node->left, out);
                writeNode(node->right, out);
            }
        };

        writeNode(root, output);
    }
    void readTree(ReadStream& input) {
        root = readTreeHelper(input);
        generateCodes(root, "");
    }

    static std::unique_ptr<HuffmanNode> readTreeHelper(ReadStream& input) {   // Рекурсивная функция (потому что работает с unique_ptr)
        char marker = input.ReadChar();
        if (marker == '1') {
            char character = input.ReadChar();
            return std::make_unique<HuffmanNode>(character, 0); // Частота не важна при декодировании
        } else if (marker == '0') {
            auto left = readTreeHelper(input);
            auto right = readTreeHelper(input);
            return std::make_unique<HuffmanNode>(0, std::move(left), std::move(right));
        } else {
            throwError(HUFFMAN_ERROR);
        }
    }

public:
    HuffmanCompressor() : root(nullptr) {}
    ~HuffmanCompressor() override = default;

    void Compress(ReadStream& input, WriteStream& output) override {
        ResetStats();
        const size_t originalSize = input.getSize();
        auto startTime = std::chrono::high_resolution_clock::now();
        if (originalSize == 0) {
            stats = CompressionStats(0, 0, 0, GetAlgorithmName());
            return;
        }
        buildFrequencyTable(input);


        // 1. Резервируем 5 байтов для заголовка
        for (int i = 0; i < 5; i++) {
            output.WriteChar(0);
        }

        // 2. Записываем дерево и запоминаем его размер
        long treeStartPos = output.getPosition();
        writeTree(output);
        long treeEndPos = output.getPosition();
        uint32_t treeSize = treeEndPos - treeStartPos;

        // 3. Кодируем данные
        input.Seek(0);
        std::string bitBuffer;
        size_t encodedBits = 0;
        while (!input.isEOF()) {
            char ch = input.ReadChar();
            std::string code = huffmanCodes[ch];
            bitBuffer += code;
            encodedBits += code.length();
            while (bitBuffer.length() >= 8) {
                std::string byteBits = bitBuffer.substr(0, 8);
                bitBuffer = bitBuffer.substr(8);
                unsigned char byte = 0;
                for (int i = 0; i < 8; i++) {
                    if (byteBits[i] == '1') {
                        byte |= (1 << (7 - i));
                    }
                }
                output.WriteChar(static_cast<char>(byte));
            }
        }

        // 4. Обрабатываем последний байт
        uint8_t significantBits = 0;
        if (!bitBuffer.empty()) {
            significantBits = bitBuffer.length();
            while (bitBuffer.length() < 8) {
                bitBuffer += "0";
            }

            unsigned char byte = 0;
            for (int i = 0; i < 8; i++) {
                if (bitBuffer[i] == '1') {
                    byte |= (1 << (7 - i));
                }
            }
            output.WriteChar(static_cast<char>(byte));
        }

        // 5. Записываем заголовок в начало
        long finalPos = output.getPosition();
        output.Seek(0);

        // Размер дерева (4 байта)
        output.WriteChar(static_cast<char>((treeSize >> 24) & 0xFF));
        output.WriteChar(static_cast<char>((treeSize >> 16) & 0xFF));
        output.WriteChar(static_cast<char>((treeSize >> 8) & 0xFF));
        output.WriteChar(static_cast<char>(treeSize & 0xFF));

        // Значимые биты (1 байт)
        output.WriteChar(static_cast<char>(significantBits));

        output.Seek(finalPos);
        output.Flush();
        size_t compressedSize = output.getSize();
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = endTime - startTime;
        stats = CompressionStats(originalSize, compressedSize, duration.count(), GetAlgorithmName());
    }
    void Decompress(ReadStream& input, WriteStream& output) override {
        ResetStats();
        size_t compressedSize = input.getSize();
        auto startTime = std::chrono::high_resolution_clock::now();
        // 1. Читаем заголовок
        uint32_t treeSize = 0;
        treeSize |= (static_cast<uint32_t>(static_cast<unsigned char>(input.ReadChar())) << 24);
        treeSize |= (static_cast<uint32_t>(static_cast<unsigned char>(input.ReadChar())) << 16);
        treeSize |= (static_cast<uint32_t>(static_cast<unsigned char>(input.ReadChar())) << 8);
        treeSize |= static_cast<uint32_t>(static_cast<unsigned char>(input.ReadChar()));

        auto significantBits = static_cast<unsigned char>(input.ReadChar());

        // 2. Читаем дерево (точно treeSize байт)
        size_t treeStartPos = input.getPosition();
        readTree(input);
        if (root == nullptr) return;

        // 3. Пропускаем оставшиеся байты дерева (если прочитали меньше чем treeSize)
        size_t treeEndPos = input.getPosition();
        if (treeEndPos - treeStartPos < treeSize) {
            input.Seek(treeStartPos + treeSize);
        }

        // 4. Декодируем данные
        HuffmanNode* currentNode = root.get();
        size_t dataStartPos = input.getPosition();
        size_t totalDataBytes = compressedSize - dataStartPos;

        // Исправление: правильная обработка significantBits
        for (size_t byteIndex = 0; byteIndex < totalDataBytes; byteIndex++) {
            auto currentByte = static_cast<unsigned char>(input.ReadChar());
            int bitsToProcess = (byteIndex == totalDataBytes - 1 && significantBits > 0)
                              ? significantBits : 8;

            // Обрабатываем биты СЛЕВА НАПРАВО (старшие биты сначала)
            for (int bitPos = 7; bitPos >= 8 - bitsToProcess; --bitPos) {
                bool bit = (currentByte >> bitPos) & 1;
                currentNode = bit ? currentNode->right.get() : currentNode->left.get();

                if (currentNode == nullptr) throwError(HUFFMAN_ERROR);

                if (currentNode->isLeaf()) {
                    output.WriteChar(currentNode->character);
                    currentNode = root.get();
                }
            }
        }
        output.Flush();
        size_t decompressedSize = output.getSize();
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = endTime - startTime;
        stats = CompressionStats(compressedSize, decompressedSize, duration.count(), GetAlgorithmName() + " (decompression)");
    }

    CompressionStats GetStats() const override { return stats; }
    std::string GetAlgorithmName() const override { return "Huffman Coding"; }
    void ResetStats() override {
        stats = CompressionStats();
        huffmanCodes.clear();
        reverseCodes.clear();
        root.reset();
    }
};

#endif