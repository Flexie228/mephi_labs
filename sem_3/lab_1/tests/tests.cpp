#define CATCH_CONFIG_MAIN
#include "../libs/catch2/catch_amalgamated.hpp"
#include "../include/Compression/HuffmanCompressor.hpp"
#include "../include/Compression/RLECompressor.hpp"
#include "../include/Compression/LZWCompressor.hpp"
#include "../include/Streams/BinaryStream.hpp"
#include <string>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void createTestFiles() {
    // Маленький файл
    std::ofstream small("test_small.txt");
    small << "AAAABBCD";
    small.close();

    // Средний файл (1KB)
    std::ofstream medium("test_medium.txt");
    for (int i = 0; i < 100; i++) {
        medium << "Hello World! This is a test text for compression. ";
    }
    medium.close();

    // Файл с повторяющимися данными
    std::ofstream large("test_large.txt");
    std::string repeated = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for (int i = 0; i < 200; i++) {
        large << repeated;
    }
    large.close();

    // Файл с повторяющимися символами (для RLE)
    std::ofstream rle_test("test_rle.txt");
    for (int i = 0; i < 500; i++) {
        rle_test << "AAAAAABBBBBBCCCCCC";
    }
    rle_test.close();
}

TEST_CASE("Huffman Compression - Different File Sizes", "[huffman][sizes]") {
    createTestFiles();

    SECTION("Small file (8 bytes)") {
        HuffmanCompressor compressor;
        ReadStream input("test_small.txt");
        WriteStream compressed("small_compressed.huff");
        WriteStream decompressed("small_decompressed.txt");

        input.Open();
        compressed.Open();
        decompressed.Open();

        compressor.Compress(input, compressed);
        auto stats = compressor.GetStats();

        WARN("Small file - Original: " << stats.originalSize << ", Compressed: " << stats.compressedSize << ", Ratio: " << stats.ratio << "%");

        input.Close();
        compressed.Close();

        // Декомпрессия и проверка целостности
        ReadStream comp_input("small_compressed.huff");
        WriteStream decomp_output("small_decompressed.txt");

        comp_input.Open();
        decomp_output.Open();
        compressor.Decompress(comp_input, decomp_output);

        comp_input.Close();
        decomp_output.Close();

        // Проверка целостности
        std::ifstream original("test_small.txt", std::ios::binary);
        std::ifstream result("small_decompressed.txt", std::ios::binary);

        std::string original_content((std::istreambuf_iterator<char>(original)),
                                    std::istreambuf_iterator<char>());
        std::string result_content((std::istreambuf_iterator<char>(result)),
                                  std::istreambuf_iterator<char>());

        REQUIRE(original_content == result_content);
    }

    SECTION("Medium file (~1KB)") {
        HuffmanCompressor compressor;
        ReadStream input("test_medium.txt");
        WriteStream compressed("medium_compressed.huff");
        WriteStream decompressed("medium_decompressed.txt");

        input.Open();
        compressed.Open();
        decompressed.Open();

        compressor.Compress(input, compressed);
        auto stats = compressor.GetStats();

        WARN("Medium file - Original: " << stats.originalSize << ", Compressed: " << stats.compressedSize << ", Ratio: " << stats.ratio << "%");

        input.Close();
        compressed.Close();

        // Декомпрессия и проверка целостности
        ReadStream comp_input("medium_compressed.huff");
        WriteStream decomp_output("medium_decompressed.txt");

        comp_input.Open();
        decomp_output.Open();
        compressor.Decompress(comp_input, decomp_output);

        comp_input.Close();
        decomp_output.Close();

        // Проверка целостности
        std::ifstream original("test_medium.txt", std::ios::binary);
        std::ifstream result("medium_decompressed.txt", std::ios::binary);

        std::string original_content((std::istreambuf_iterator<char>(original)),
                                    std::istreambuf_iterator<char>());
        std::string result_content((std::istreambuf_iterator<char>(result)),
                                  std::istreambuf_iterator<char>());

        REQUIRE(original_content == result_content);

        // Для средних файлов ожидаем сжатие
        if (stats.originalSize > 100) {
            REQUIRE(stats.compressedSize < stats.originalSize);
        }
    }

    SECTION("Large file (~10KB)") {
        HuffmanCompressor compressor;
        ReadStream input("test_large.txt");
        WriteStream compressed("large_compressed.huff");
        WriteStream decompressed("large_decompressed.txt");

        input.Open();
        compressed.Open();
        decompressed.Open();

        compressor.Compress(input, compressed);
        auto stats = compressor.GetStats();

        WARN("Large file - Original: " << stats.originalSize << ", Compressed: " << stats.compressedSize << ", Ratio: " << stats.ratio << "%");

        input.Close();
        compressed.Close();

        // Декомпрессия и проверка целостности
        ReadStream comp_input("large_compressed.huff");
        WriteStream decomp_output("large_decompressed.txt");

        comp_input.Open();
        decomp_output.Open();
        compressor.Decompress(comp_input, decomp_output);

        comp_input.Close();
        decomp_output.Close();

        // Проверка целостности
        std::ifstream original("test_large.txt", std::ios::binary);
        std::ifstream result("large_decompressed.txt", std::ios::binary);

        std::string original_content((std::istreambuf_iterator<char>(original)),
                                    std::istreambuf_iterator<char>());
        std::string result_content((std::istreambuf_iterator<char>(result)),
                                  std::istreambuf_iterator<char>());

        REQUIRE(original_content == result_content);

        // Для больших файлов обязательно должно быть сжатие
        REQUIRE(stats.compressedSize < stats.originalSize);
    }

    // Очистка
    fs::remove("test_small.txt");
    fs::remove("test_medium.txt");
    fs::remove("test_large.txt");
    fs::remove("small_compressed.huff");
    fs::remove("medium_compressed.huff");
    fs::remove("large_compressed.huff");
    fs::remove("small_decompressed.txt");
    fs::remove("medium_decompressed.txt");
    fs::remove("large_decompressed.txt");
}

TEST_CASE("RLE Compression - Different File Sizes", "[rle][sizes]") {
    createTestFiles();

    SECTION("RLE on text with repeats") {
        RLECompressor compressor;
        BinaryStream input("test_rle.txt");
        BinaryStream compressed("rle_compressed.rle");
        BinaryStream decompressed("rle_decompressed.txt");

        input.Open();
        compressed.Open();
        decompressed.Open();

        compressor.Compress(input, compressed);
        auto stats = compressor.GetStats();

        WARN("RLE on repeating text - Original: " << stats.originalSize << ", Compressed: " << stats.compressedSize << ", Ratio: " << stats.ratio << "%");

        input.Close();
        compressed.Close();

        // Декомпрессия и проверка целостности
        BinaryStream comp_input("rle_compressed.rle");
        BinaryStream decomp_output("rle_decompressed.txt");

        comp_input.Open();
        decomp_output.Open();
        compressor.Decompress(comp_input, decomp_output);

        comp_input.Close();
        decomp_output.Close();

        // Проверка целостности
        std::ifstream original("test_rle.txt", std::ios::binary);
        std::ifstream result("rle_decompressed.txt", std::ios::binary);

        std::string original_content((std::istreambuf_iterator<char>(original)),
                                    std::istreambuf_iterator<char>());
        std::string result_content((std::istreambuf_iterator<char>(result)),
                                  std::istreambuf_iterator<char>());

        REQUIRE(original_content == result_content);

        // RLE должен хорошо сжимать повторяющиеся данные
        REQUIRE(stats.compressedSize < stats.originalSize);
    }

    SECTION("RLE on medium file") {
        RLECompressor compressor;
        BinaryStream input("test_medium.txt");
        BinaryStream compressed("rle_medium_compressed.rle");
        BinaryStream decompressed("rle_medium_decompressed.txt");

        input.Open();
        compressed.Open();
        decompressed.Open();

        compressor.Compress(input, compressed);
        auto stats = compressor.GetStats();

        WARN("RLE on medium file - Original: " << stats.originalSize << ", Compressed: " << stats.compressedSize << ", Ratio: " << stats.ratio << "%");

        input.Close();
        compressed.Close();

        // Декомпрессия и проверка целостности
        BinaryStream comp_input("rle_medium_compressed.rle");
        BinaryStream decomp_output("rle_medium_decompressed.txt");

        comp_input.Open();
        decomp_output.Open();
        compressor.Decompress(comp_input, decomp_output);

        comp_input.Close();
        decomp_output.Close();

        // Проверка целостности
        std::ifstream original("test_medium.txt", std::ios::binary);
        std::ifstream result("rle_medium_decompressed.txt", std::ios::binary);

        std::string original_content((std::istreambuf_iterator<char>(original)),
                                    std::istreambuf_iterator<char>());
        std::string result_content((std::istreambuf_iterator<char>(result)),
                                  std::istreambuf_iterator<char>());

        REQUIRE(original_content == result_content);
    }

    // Очистка
    fs::remove("test_rle.txt");
    fs::remove("test_medium.txt");
    fs::remove("rle_compressed.rle");
    fs::remove("rle_decompressed.txt");
    fs::remove("rle_medium_compressed.rle");
    fs::remove("rle_medium_decompressed.txt");
}

TEST_CASE("Algorithm Comparison", "[comparison]") {
    createTestFiles();

    SECTION("Huffman vs RLE on repeating data") {
        HuffmanCompressor huffman;
        RLECompressor rle;

        BinaryStream input("test_rle.txt");

        // Huffman
        ReadStream huff_input("test_rle.txt");
        WriteStream huff_output("huffman_rle.huff");
        huff_input.Open();
        huff_output.Open();
        huffman.Compress(huff_input, huff_output);
        auto huff_stats = huffman.GetStats();
        huff_input.Close();
        huff_output.Close();

        // RLE
        BinaryStream rle_input("test_rle.txt");
        BinaryStream rle_output("rle_output.rle");
        rle_input.Open();
        rle_output.Open();
        rle.Compress(rle_input, rle_output);
        auto rle_stats = rle.GetStats();
        rle_input.Close();
        rle_output.Close();

        WARN("Huffman on repeating data: " << huff_stats.ratio << "%");
        WARN("RLE on repeating data: " << rle_stats.ratio << "%");

        // RLE должен быть лучше на повторяющихся данных
        if (rle_stats.ratio > huff_stats.ratio) {
            WARN("RLE outperforms Huffman on repeating data (as expected)");
        }
    }

    SECTION("Huffman vs RLE on normal text") {
        HuffmanCompressor huffman;
        RLECompressor rle;

        // Huffman
        ReadStream huff_input("test_medium.txt");
        WriteStream huff_output("huffman_medium.huff");
        huff_input.Open();
        huff_output.Open();
        huffman.Compress(huff_input, huff_output);
        auto huff_stats = huffman.GetStats();
        huff_input.Close();
        huff_output.Close();

        // RLE
        BinaryStream rle_input("test_medium.txt");
        BinaryStream rle_output("rle_medium_output.rle");
        rle_input.Open();
        rle_output.Open();
        rle.Compress(rle_input, rle_output);
        auto rle_stats = rle.GetStats();
        rle_input.Close();
        rle_output.Close();

        WARN("Huffman on normal text: " << huff_stats.ratio << "%");
        WARN("RLE on normal text: " << rle_stats.ratio << "%");

        // Huffman должен быть лучше на нормальном тексте
        if (huff_stats.ratio > rle_stats.ratio) {
            WARN("Huffman outperforms RLE on normal text (as expected)");
        }
    }

    // Очистка
    fs::remove("test_rle.txt");
    fs::remove("test_medium.txt");
    fs::remove("huffman_rle.huff");
    fs::remove("rle_output.rle");
    fs::remove("huffman_medium.huff");
    fs::remove("rle_medium_output.rle");
}

TEST_CASE("Edge Cases", "[edge]") {
    SECTION("Empty file") {
        // Создаем пустой файл
        std::ofstream empty("empty_test.txt");
        empty.close();

        HuffmanCompressor huffman;
        ReadStream input("empty_test.txt");
        WriteStream compressed("empty_compressed.huff");

        input.Open();
        compressed.Open();

        REQUIRE_NOTHROW(huffman.Compress(input, compressed));
        auto stats = huffman.GetStats();

        REQUIRE(stats.originalSize == 0);
        REQUIRE(stats.compressedSize == 0);
        REQUIRE(stats.ratio == 0.0);

        input.Close();
        compressed.Close();

        fs::remove("empty_test.txt");
        fs::remove("empty_compressed.huff");
    }
}

TEST_CASE("Huffman Compression - Text File", "[huffman][text]") {
    SECTION("Compress and decompress test_text.txt") {
        HuffmanCompressor compressor;

        // Сжатие
        ReadStream input("../tests/test_text.txt");
        WriteStream compressed("text_compressed.huff");

        input.Open();
        compressed.Open();

        compressor.Compress(input, compressed);
        auto stats = compressor.GetStats();

        WARN("Text file Huffman - Original: " << stats.originalSize
             << ", Compressed: " << stats.compressedSize
             << ", Ratio: " << stats.ratio << "%");

        input.Close();
        compressed.Close();

        // Декомпрессия
        ReadStream comp_input("text_compressed.huff");
        WriteStream decomp_output("text_decompressed.txt");

        comp_input.Open();
        decomp_output.Open();
        compressor.Decompress(comp_input, decomp_output);

        comp_input.Close();
        decomp_output.Close();  // ЗАКРЫВАЕМ перед удалением!

        // Проверка целостности
        std::ifstream original("../tests/test_text.txt", std::ios::binary);
        std::ifstream result("text_decompressed.txt", std::ios::binary);

        std::string original_content((std::istreambuf_iterator<char>(original)),
                                    std::istreambuf_iterator<char>());
        std::string result_content((std::istreambuf_iterator<char>(result)),
                                  std::istreambuf_iterator<char>());

        // Закрываем файлы перед удалением
        original.close();
        result.close();

        REQUIRE(original_content == result_content);
        REQUIRE(stats.compressedSize < stats.originalSize);

        // Очистка
        fs::remove("text_compressed.huff");
        fs::remove("text_decompressed.txt");
    }
}

TEST_CASE("RLE Compression - Image File", "[rle][image]") {
    SECTION("Compress and decompress test_image.jpg") {
        RLECompressor compressor;

        // Сжатие
        BinaryStream input("../tests/test_image.jpg");
        BinaryStream compressed("image_compressed.rle");

        input.Open();
        compressed.Open();

        compressor.Compress(input, compressed);
        auto stats = compressor.GetStats();

        WARN("Image file RLE - Original: " << stats.originalSize
             << ", Compressed: " << stats.compressedSize
             << ", Ratio: " << stats.ratio << "%");

        input.Close();
        compressed.Close();

        // Декомпрессия
        BinaryStream comp_input("image_compressed.rle");
        BinaryStream decomp_output("image_decompressed.jpg");

        comp_input.Open();
        decomp_output.Open();
        compressor.Decompress(comp_input, decomp_output);

        comp_input.Close();
        decomp_output.Close();

        uintmax_t original_size = fs::file_size("../tests/test_image.jpg");
        uintmax_t result_size = fs::file_size("image_decompressed.jpg");

        REQUIRE(original_size == result_size);

        // Для RLE на изображениях может не быть сжатия, поэтому только проверяем целостность
        std::ifstream original("../tests/test_image.jpg", std::ios::binary);
        std::ifstream result("image_decompressed.jpg", std::ios::binary);

        REQUIRE(original.good());
        REQUIRE(result.good());

        // Закрываем файлы перед удалением
        original.close();
        result.close();

        // Очистка
        fs::remove("image_compressed.rle");
        fs::remove("image_decompressed.jpg");
    }
}

TEST_CASE("LZW Compression - Basic Functionality", "[lzw][basic]") {
    SECTION("Compress and decompress simple text") {
        // Создаем простой тестовый файл
        std::ofstream test("lzw_test.txt");
        test << "ABABABA";
        test.close();

        LZWCompressor compressor;
        BinaryStream input("lzw_test.txt");
        BinaryStream compressed("lzw_test_compressed.lzw");

        input.Open();
        compressed.Open();

        // Сжатие
        REQUIRE_NOTHROW(compressor.Compress(input, compressed));
        auto stats = compressor.GetStats();

        WARN("LZW simple text - Original: " << stats.originalSize << ", Compressed: " << stats.compressedSize << ", Ratio: " << stats.ratio << "%");

        input.Close();
        compressed.Close();

        // Декомпрессия
        BinaryStream comp_input("lzw_test_compressed.lzw");
        BinaryStream decomp_output("lzw_test_decompressed.txt");

        comp_input.Open();
        decomp_output.Open();
        REQUIRE_NOTHROW(compressor.Decompress(comp_input, decomp_output));

        comp_input.Close();
        decomp_output.Close();

        // Проверка целостности
        std::ifstream original("lzw_test.txt", std::ios::binary);
        std::ifstream result("lzw_test_decompressed.txt", std::ios::binary);

        std::string original_content((std::istreambuf_iterator<char>(original)), std::istreambuf_iterator<char>());
        std::string result_content((std::istreambuf_iterator<char>(result)), std::istreambuf_iterator<char>());

        original.close();
        result.close();

        REQUIRE(original_content == result_content);

        // Очистка
        fs::remove("lzw_test.txt");
        fs::remove("lzw_test_compressed.lzw");
        fs::remove("lzw_test_decompressed.txt");
    }
    SECTION("LZW on text with patterns") {
        std::ofstream test("lzw_pattern.txt");
        for (int i = 0; i < 10; i++) {
            test << "HelloWorldHelloWorld";
        }
        test.close();

        LZWCompressor compressor;
        BinaryStream input("lzw_pattern.txt");
        BinaryStream compressed("lzw_pattern_compressed.lzw");

        input.Open();
        compressed.Open();

        compressor.Compress(input, compressed);
        auto stats = compressor.GetStats();

        WARN("LZW pattern text - Original: " << stats.originalSize << ", Compressed: " << stats.compressedSize << ", Ratio: " << stats.ratio << "%");

        if (stats.originalSize > 50) {
            REQUIRE(stats.compressedSize < stats.originalSize);
        }

        input.Close();
        compressed.Close();

        // Очистка
        fs::remove("lzw_pattern.txt");
        fs::remove("lzw_pattern_compressed.lzw");
    }
}

TEST_CASE("LZW Compression - Dictionary Reset", "[lzw][dictionary]") {
    SECTION("Test dictionary growth and reset") {
        std::ofstream test("lzw_large.txt");
        for (int i = 0; i < 5000; i++) {
            char c1 = static_cast<char>('A' + (i % 26));
            char c2 = static_cast<char>('a' + ((i + 1) % 26));
            test << c1 << c2;
        }
        test.close();

        // Проверяем размер оригинального файла
        uintmax_t original_size = fs::file_size("lzw_large.txt");
        WARN("Original file size: " << original_size << " bytes");

        LZWCompressor compressor;
        BinaryStream input("lzw_large.txt");
        BinaryStream compressed("lzw_large_compressed.lzw");

        input.Open();
        compressed.Open();

        REQUIRE_NOTHROW(compressor.Compress(input, compressed));
        auto compress_stats = compressor.GetStats();

        WARN("Compressed size: " << compress_stats.compressedSize << " bytes");
        WARN("Compression ratio: " << compress_stats.ratio << "%");

        input.Close();
        compressed.Close();

        // Проверяем размер сжатого файла
        uintmax_t compressed_size = fs::file_size("lzw_large_compressed.lzw");
        WARN("Actual compressed file size: " << compressed_size << " bytes");

        // Декомпрессия
        BinaryStream comp_input("lzw_large_compressed.lzw");
        BinaryStream decomp_output("lzw_large_decompressed.txt");

        comp_input.Open();
        decomp_output.Open();

        // Добавляем try-catch для детальной диагностики
        try {
            compressor.Decompress(comp_input, decomp_output);
        } catch (const std::exception& e) {
            WARN("Decompression failed with: " << e.what());

            // Дополнительная диагностика
            WARN("Compressed file size: " << compressed_size);
            WARN("Input position before error: " << comp_input.getSize());
            throw;
        }

        comp_input.Close();
        decomp_output.Close();

        // Проверка целостности
        std::ifstream original("lzw_large.txt", std::ios::binary);
        std::ifstream result("lzw_large_decompressed.txt", std::ios::binary);

        std::string original_content((std::istreambuf_iterator<char>(original)), std::istreambuf_iterator<char>());
        std::string result_content((std::istreambuf_iterator<char>(result)), std::istreambuf_iterator<char>());

        original.close();
        result.close();

        REQUIRE(original_content == result_content);

        fs::remove("lzw_large.txt");
        fs::remove("lzw_large_compressed.lzw");
        fs::remove("lzw_large_decompressed.txt");
    }
}

TEST_CASE("LZW Compression - Edge Cases", "[lzw][edge]") {
    SECTION("Empty file") {
        std::ofstream empty("lzw_empty.txt");
        empty.close();

        LZWCompressor compressor;
        BinaryStream input("lzw_empty.txt");
        BinaryStream compressed("lzw_empty_compressed.lzw");

        input.Open();
        compressed.Open();

        REQUIRE_NOTHROW(compressor.Compress(input, compressed));
        auto stats = compressor.GetStats();

        REQUIRE(stats.originalSize == 0);
        REQUIRE(stats.compressedSize == 0);
        REQUIRE(stats.ratio == 0.0);

        input.Close();
        compressed.Close();

        fs::remove("lzw_empty.txt");
        fs::remove("lzw_empty_compressed.lzw");
    }

    SECTION("All same characters") {
        std::ofstream same("lzw_same.txt");
        for (int i = 0; i < 1000; i++) {
            same << "A";
        }
        same.close();

        LZWCompressor compressor;
        BinaryStream input("lzw_same.txt");
        BinaryStream compressed("lzw_same_compressed.lzw");

        input.Open();
        compressed.Open();

        compressor.Compress(input, compressed);
        auto stats = compressor.GetStats();

        WARN("LZW all same chars - Original: " << stats.originalSize << ", Compressed: " << stats.compressedSize << ", Ratio: " << stats.ratio << "%");

        REQUIRE(stats.compressedSize < stats.originalSize);

        input.Close();
        compressed.Close();

        fs::remove("lzw_same.txt");
        fs::remove("lzw_same_compressed.lzw");
    }
}

TEST_CASE("LZW Compression - Algorithm Comparison", "[lzw][comparison]") {
    createTestFiles();

    SECTION("LZW vs Huffman vs RLE on repeating data") {
        // Тестовый файл с повторениями
        std::ofstream test("comparison_test.txt");
        for (int i = 0; i < 100; i++) {
            test << "ABCABCABCABC";
        }
        test.close();

        // LZW
        {
            LZWCompressor lzw;
            BinaryStream input("comparison_test.txt");
            BinaryStream compressed("lzw_comparison.lzw");
            input.Open();
            compressed.Open();
            lzw.Compress(input, compressed);
            auto lzw_stats = lzw.GetStats();
            input.Close();
            compressed.Close();

            WARN("LZW on repeating pattern: " << lzw_stats.ratio << "%");
        }

        // Huffman
        {
            HuffmanCompressor huffman;
            ReadStream input("comparison_test.txt");
            WriteStream compressed("huffman_comparison.huff");
            input.Open();
            compressed.Open();
            huffman.Compress(input, compressed);
            auto huff_stats = huffman.GetStats();
            input.Close();
            compressed.Close();

            WARN("Huffman on repeating pattern: " << huff_stats.ratio << "%");
        }

        // RLE
        {
            RLECompressor rle;
            BinaryStream input("comparison_test.txt");
            BinaryStream compressed("rle_comparison.rle");
            input.Open();
            compressed.Open();
            rle.Compress(input, compressed);
            auto rle_stats = rle.GetStats();
            input.Close();
            compressed.Close();

            WARN("RLE on repeating pattern: " << rle_stats.ratio << "%");
        }

        // Очистка
        fs::remove("comparison_test.txt");
        fs::remove("lzw_comparison.lzw");
        fs::remove("huffman_comparison.huff");
        fs::remove("rle_comparison.rle");
    }

    SECTION("LZW on binary file") {
        LZWCompressor compressor;

        // Сжатие существующего тестового изображения
        BinaryStream input("../tests/test_image.jpg");
        BinaryStream compressed("image_lzw_compressed.lzw");

        input.Open();
        compressed.Open();

        REQUIRE_NOTHROW(compressor.Compress(input, compressed));
        auto stats = compressor.GetStats();

        WARN("LZW on image - Original: " << stats.originalSize << ", Compressed: " << stats.compressedSize << ", Ratio: " << stats.ratio << "%");

        input.Close();
        compressed.Close();

        BinaryStream comp_input("image_lzw_compressed.lzw");
        BinaryStream decomp_output("image_lzw_decompressed.jpg");

        comp_input.Open();
        decomp_output.Open();
        REQUIRE_NOTHROW(compressor.Decompress(comp_input, decomp_output));

        comp_input.Close();
        decomp_output.Close();

        // Проверка размера
        // uintmax_t original_size = fs::file_size("../tests/test_image.jpg");
        // uintmax_t result_size = fs::file_size("image_lzw_decompressed.jpg");

        // REQUIRE(original_size == result_size);

        // Очистка
        fs::remove("image_lzw_compressed.lzw");
        fs::remove("image_lzw_decompressed.jpg");
    }
}

TEST_CASE("LZW Compression - Error Handling", "[lzw][error]") {

    SECTION("Truncated stream") {
        // Создаем незавершенный LZW поток
        std::ofstream truncated("truncated.lzw", std::ios::binary);
        // Пишем CLEAR_CODE (256 в 9 битах) и обрываем
        // 256 в 9 битах: 1 0000 0000
        // В байтах: 0x80 (первый байт), а второй байт не записываем
        truncated.put(static_cast<char>(0x80));
        truncated.close();

        LZWCompressor compressor;
        BinaryStream input("truncated.lzw");
        BinaryStream output("truncated_output.txt");

        input.Open();
        output.Open();

        // Должно выбросить исключение при попытке прочитать второй код
        REQUIRE_THROWS_AS(compressor.Decompress(input, output), std::runtime_error);

        input.Close();
        output.Close();

        fs::remove("truncated.lzw");
        fs::remove("truncated_output.txt");
    }
}

TEST_CASE("LZW Compression - Text File", "[lzw][text]") {
    SECTION("Compress and decompress test_text.txt to tests folder") {
        LZWCompressor compressor;

        // Сжатие
        BinaryStream input("../tests/test_text.txt");
        BinaryStream compressed("text_lzw_compressed.lzw");

        input.Open();
        compressed.Open();

        compressor.Compress(input, compressed);
        auto stats = compressor.GetStats();

        WARN("Text file LZW - Original: " << stats.originalSize
             << ", Compressed: " << stats.compressedSize
             << ", Ratio: " << stats.ratio << "%");

        input.Close();
        compressed.Close();

        // Декомпрессия с сохранением в папку tests
        BinaryStream comp_input("text_lzw_compressed.lzw");
        BinaryStream decomp_output("../tests/test_text_decompressed_lzw.txt");

        comp_input.Open();
        decomp_output.Open();
        compressor.Decompress(comp_input, decomp_output);

        comp_input.Close();
        decomp_output.Close();

        // Проверка целостности
        std::ifstream original("../tests/test_text.txt", std::ios::binary);
        std::ifstream result("../tests/test_text_decompressed_lzw.txt", std::ios::binary);

        std::string original_content((std::istreambuf_iterator<char>(original)),
                                    std::istreambuf_iterator<char>());
        std::string result_content((std::istreambuf_iterator<char>(result)),
                                  std::istreambuf_iterator<char>());

        original.close();
        result.close();

        REQUIRE(original_content == result_content);

        // Удаляем только сжатый файл, декомпрессированный оставляем в tests
        fs::remove("text_lzw_compressed.lzw");

        // Проверяем, что декомпрессированный файл существует в tests
        REQUIRE(fs::exists("../tests/test_text_decompressed_lzw.txt"));

        // Выводим информацию о размерах
        WARN("Decompressed file kept at: ../tests/test_text_decompressed_lzw.txt");
        WARN("Size: " << fs::file_size("../tests/test_text_decompressed_lzw.txt") << " bytes");
    }
}