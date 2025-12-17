#include "../libs/catch2/catch_amalgamated.hpp"
#include "../include/Streams/Stream.hpp"
#include "../include/Streams/ReadStream.hpp"
#include "../include/Streams/WriteStream.hpp"
#include "../include/Streams/ReadWriteStream.hpp"
#include "../include/Streams/BinaryStream.hpp"
#include "../include/Exceptions.hpp"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

TEST_CASE("WriteStream", "[write]") {
    const std::string filename = "test_write.txt";
    
    // Cleanup before test
    std::error_code ec;
    fs::remove(filename, ec);

    SECTION("Open and Close") {
        WriteStream stream(filename);
        REQUIRE_FALSE(stream.isOpen());
        
        stream.Open();
        REQUIRE(stream.isOpen());
        REQUIRE(stream.isWrite());
        
        stream.Close();
        REQUIRE_FALSE(stream.isOpen());
    }

    SECTION("Write single characters") {
        WriteStream stream(filename);
        stream.Open();
        
        stream.WriteChar('H');
        stream.WriteChar('e');
        stream.WriteChar('l');
        stream.WriteChar('l');
        stream.WriteChar('o');
        
        stream.Close();
        
        // Verify file content
        std::ifstream file(filename);
        std::string content((std::istreambuf_iterator<char>(file)), 
                           std::istreambuf_iterator<char>());
        REQUIRE(content == "Hello");
    }

    SECTION("Write strings") {
        WriteStream stream(filename);
        stream.Open();
        
        stream.WriteString("Hello");
        stream.WriteString(" ");
        stream.WriteString("World!");
        
        stream.Close();
        
        std::ifstream file(filename);
        std::string content((std::istreambuf_iterator<char>(file)), 
                           std::istreambuf_iterator<char>());
        REQUIRE(content == "Hello World!");
    }

    SECTION("Seek and write") {
        WriteStream stream(filename);
        stream.Open();

        stream.WriteString("ABCDEFGH");
        stream.Close();  // Закрываем и открываем заново для чтения

        // Проверяем через ReadStream
        ReadStream readStream(filename);
        readStream.Open();
        std::string content = readStream.ReadTextFile();
        REQUIRE(content == "ABCDEFGH");

        // Или используй ReadWriteStream для смешанных операций
        ReadWriteStream rwStream(filename);
        rwStream.Open();
        rwStream.Seek(2);
        rwStream.WriteString("XY");
        rwStream.Close();

        // Проверяем результат
        ReadStream verifyStream(filename);
        verifyStream.Open();
        std::string result = verifyStream.ReadTextFile();
        REQUIRE(result == "ABXYEFGH");
    }

    SECTION("Flush operation") {
        WriteStream stream(filename);
        stream.Open();
        
        stream.WriteString("Test data");
        stream.Flush();
        
        stream.Close();
    }

    // Cleanup
    std::error_code ecf;
    fs::remove(filename, ecf);
}

TEST_CASE("ReadStream", "[read]") {
    const std::string filename = "test_read.txt";
    
    // Setup test file
    {
        std::ofstream file(filename);
        file << "Hello World!\nThis is a test file.\nAnother line.";
        file.close();
    }

    SECTION("Open and Close") {
        ReadStream stream(filename);
        REQUIRE_FALSE(stream.isOpen());
        
        stream.Open();
        REQUIRE(stream.isOpen());
        REQUIRE_FALSE(stream.isWrite());
        
        stream.Close();
        REQUIRE_FALSE(stream.isOpen());
    }

    SECTION("Read single characters") {
        ReadStream stream(filename);
        stream.Open();
        
        REQUIRE(stream.ReadChar() == 'H');
        REQUIRE(stream.ReadChar() == 'e');
        REQUIRE(stream.ReadChar() == 'l');
        REQUIRE(stream.ReadChar() == 'l');
        REQUIRE(stream.ReadChar() == 'o');
        
        stream.Close();
    }

    SECTION("Read words") {
        ReadStream stream(filename);
        stream.Open();
        
        REQUIRE(stream.ReadWord() == "Hello");
        REQUIRE(stream.ReadWord() == "World");
        REQUIRE(stream.ReadWord() == "This");
        REQUIRE(stream.ReadWord() == "is");
        REQUIRE(stream.ReadWord() == "a");
        REQUIRE(stream.ReadWord() == "test");
        REQUIRE(stream.ReadWord() == "file");
        REQUIRE(stream.ReadWord() == "Another");
        REQUIRE(stream.ReadWord() == "line");
        
        stream.Close();
    }

    SECTION("Read lines") {
        ReadStream stream(filename);
        stream.Open();

        REQUIRE(stream.ReadLine() == "Hello World!");
        REQUIRE(stream.ReadLine() == "This is a test file.");
        REQUIRE(stream.ReadLine() == "Another line.");

        stream.Close();
    }

    SECTION("Read entire file") {
        ReadStream stream(filename);
        stream.Open();
        
        std::string content = stream.ReadTextFile();
        REQUIRE(content.find("Hello World!") != std::string::npos);
        REQUIRE(content.find("This is a test file.") != std::string::npos);
        REQUIRE(content.find("Another line.") != std::string::npos);
        
        stream.Close();
    }

    SECTION("Seek operation") {
        ReadStream stream(filename);
        stream.Open();
        
        stream.Seek(6);
        REQUIRE(stream.ReadChar() == 'W');
        REQUIRE(stream.ReadChar() == 'o');
        
        stream.Seek(0);
        REQUIRE(stream.ReadChar() == 'H');
        
        stream.Close();
    }

    SECTION("EOF detection") {
        ReadStream stream(filename);
        stream.Open();
        
        // Read entire file
        while (!stream.isEOF()) {
            stream.ReadChar();
        }
        
        REQUIRE(stream.isEOF());
        
        stream.Close();
    }

    std::error_code ec;
    fs::remove(filename, ec);
}

TEST_CASE("BinaryStream", "[binary]") {
    const std::string filename = "test_binary.bin";
    
    std::error_code ec;
    fs::remove(filename, ec);

    SECTION("Binary read/write operations") {
        BinaryStream stream(filename);
        stream.Open();
        
        // Write binary data
        stream.WriteByte(0x41); // 'A'
        stream.WriteByte(0x42); // 'B'
        stream.WriteByte(0x43); // 'C'
        
        // Read back
        stream.Seek(0);
        REQUIRE(stream.ReadByte() == 0x41);
        REQUIRE(stream.ReadByte() == 0x42);
        REQUIRE(stream.ReadByte() == 0x43);
        
        stream.Close();
    }

    SECTION("Block read/write operations") {
        BinaryStream stream(filename);
        stream.Open();

        constexpr char writeData[] = "Binary block data";
        constexpr size_t dataSize = sizeof(writeData) - 1; // exclude null terminator
        
        // Write block
        size_t written = stream.WriteBlock(writeData, dataSize);
        REQUIRE(written == dataSize);
        
        // Read block
        stream.Seek(0);
        char readData[50] = {0};
        size_t read = stream.ReadBlock(readData, dataSize);
        REQUIRE(read == dataSize);
        REQUIRE(std::string(readData) == std::string(writeData));
        
        stream.Close();
    }

    SECTION("Mode switching") {
        BinaryStream stream(filename);
        stream.Open();
        
        REQUIRE_FALSE(stream.isWriteMode()); // Starts in read mode
        
        stream.WriteByte(0x50);
        REQUIRE(stream.isWriteMode());

        stream.Flush();
        stream.Seek(0);
        
        char result = stream.ReadByte();
        REQUIRE_FALSE(stream.isWriteMode());
        REQUIRE(result == 0x50);
        
        stream.Close();
    }

    // Cleanup
    std::error_code ecf;
    fs::remove(filename, ecf);
}

TEST_CASE("ReadWriteStream", "[readwrite]") {
    const std::string filename = "test_readwrite.txt";
    
    // Cleanup before test
    std::error_code ec;
    fs::remove(filename, ec);

    SECTION("Mixed read/write operations") {
        ReadWriteStream stream(filename);
        stream.Open();
        
        // Write some data
        stream.WriteString("Initial data");
        
        // Read it back
        stream.Seek(0);
        REQUIRE(stream.ReadChar() == 'I');
        REQUIRE(stream.ReadWord() == "nitial");
        REQUIRE(stream.ReadWord() == "data");
        
        // Write more data
        stream.WriteString(" appended");
        
        // Read everything
        stream.Seek(0);
        std::string content = stream.ReadLine();
        REQUIRE(content == "Initial data appended");
        
        stream.Close();
    }

    SECTION("Seek and overwrite") {
        ReadWriteStream stream(filename);
        stream.Open();

        stream.WriteString("ABCDEFGH");
        stream.Flush();  // ← ЗАПИСЫВАЕМ БУФЕР

        stream.Seek(2);
        stream.WriteString("XY");
        stream.Flush();  // ← ЗАПИСЫВАЕМ БУФЕР

        stream.Seek(0);
        REQUIRE(stream.ReadLine() == "ABXYEFGH");

        stream.Close();
    }

    // Cleanup
    std::error_code ecf;
    fs::remove(filename, ecf);
}

TEST_CASE("Error handling", "[errors]") {
    SECTION("Empty filename") {
        WriteStream stream("");
        REQUIRE_THROWS_AS(stream.Open(), std::runtime_error);
    }

    SECTION("File not opened operations") {
        ReadStream stream("nonexistent.txt");
        REQUIRE_THROWS_AS(stream.ReadChar(), std::runtime_error);
        REQUIRE_THROWS_AS(stream.Seek(0), std::runtime_error);
    }

    SECTION("Seek beyond file size") {
        const std::string filename = "test_seek.txt";
        
        // Create small file
        {
            std::ofstream file(filename);
            file << "ABC";
            file.close();
        }
        
        ReadStream stream(filename);
        stream.Open();
        
        REQUIRE_THROWS_AS(stream.Seek(100), std::runtime_error);
        
        stream.Close();
        
        // Cleanup
        std::error_code ec;
        fs::remove(filename, ec);
    }

    SECTION("Read beyond EOF") {
        const std::string filename = "test_eof.txt";
        
        // Create small file
        {
            std::ofstream file(filename);
            file << "A";
            file.close();
        }
        
        ReadStream stream(filename);
        stream.Open();
        
        REQUIRE(stream.ReadChar() == 'A');
        REQUIRE_THROWS_AS(stream.ReadChar(), std::runtime_error);
        
        stream.Close();
        
        // Cleanup
        std::error_code ec;
        fs::remove(filename, ec);
    }
}

TEST_CASE("File size and position", "[filesize]") {
    const std::string filename = "test_size.txt";
    
    std::error_code ec;
    fs::remove(filename, ec);

    SECTION("File size tracking") {
        WriteStream stream(filename);
        stream.Open();
        
        REQUIRE(stream.getSize() == 0);
        
        stream.WriteString("Hello World");
        stream.Flush();
        REQUIRE(stream.getSize() == 11);
        
        stream.WriteString("!");
        stream.Flush();
        REQUIRE(stream.getSize() == 12);
        
        stream.Close();
        
        // Verify with filesystem
        REQUIRE(fs::file_size(filename) == 12);
    }

    SECTION("Position tracking") {
        WriteStream stream(filename);
        stream.Open();
        
        REQUIRE(stream.getPosition() == 0);
        
        stream.WriteString("ABCD");
        REQUIRE(stream.getPosition() == 4);
        
        stream.Close();

        ReadStream readStream(filename);
        readStream.Open();
        std::string content = readStream.ReadTextFile();
        REQUIRE(content == "ABCD");
        readStream.Close();
    }

    std::error_code ecf;
    fs::remove(filename, ecf);
}