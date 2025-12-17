#ifndef BINARY_STREAM
#define BINARY_STREAM

#include "Stream.hpp"
#include "../Exceptions.hpp"
#include <cstring>

class BinaryStream final : public Stream {
private:
    void fillBuffer() {
        if (!file) return;
        bufferSize = fread(buffer, sizeof(char), BUFFER_SIZE, file);
        bufferPosition = 0;
    }

    void writeFromBuffer() {
        if (bufferPosition > 0 && file) {
            size_t written = fwrite(buffer, sizeof(char), bufferPosition, file);
            if (written != static_cast<size_t>(bufferPosition)) {
                throwError(FILE_WRITE_ERROR);
            }
            bufferPosition = 0;
        }
    }

    void flushWrite() {
        streamPosition += bufferPosition;
        writeFromBuffer();
        if (file) {
            fflush(file);
        }
    }

    void switchToReadMode() {
        if (WriteMode) {
            writeFromBuffer();
            WriteMode = false;
            fseek(file, static_cast<long>(streamPosition), SEEK_SET);
            fillBuffer();
        }
    }

    void switchToWriteMode() {
        if (!WriteMode) {
            bufferPosition = 0;
            bufferSize = 0;
            WriteMode = true;
            fseek(file, static_cast<long>(streamPosition), SEEK_SET);
        }
    }

public:
    explicit BinaryStream(const string& filename) : Stream(filename) {}
    ~BinaryStream() override = default;

    void Open() override {
        if (isOpen()) return;
        if (filename.empty()) throwError(FILE_EMPTY);

        file = fopen(filename.c_str(), "rb+");
        if (!file) {
            file = fopen(filename.c_str(), "wb+");
            if (!file) throwError(FILE_OPEN_ERROR);
        }

        OpenFlag = true;
        streamPosition = 0;
        WriteMode = false;
        fillBuffer();
    }

    void Close() override {
        if (!isOpen()) return;

        if (WriteMode && bufferPosition > 0) {
            writeFromBuffer();
        }

        if (file) {
            fclose(file);
            file = nullptr;
        }

        OpenFlag = false;
        bufferPosition = 0;
        bufferSize = 0;
        streamPosition = 0;
    }

    size_t Seek(const size_t pos) override {
        if (!isOpen()) throwError(FILE_NOT_OPENED);
        if (static_cast<long>(pos) > getSize()) throwError(SEEK_ERROR);

        if (WriteMode) {
            writeFromBuffer();
        } else {
            bufferPosition = 0;
            bufferSize = 0;
        }

        int result = fseek(file, static_cast<long>(pos), SEEK_SET);
        if (result != 0) throwError(SEEK_ERROR);

        streamPosition = pos;
        WriteMode = false;
        fillBuffer();
        return streamPosition;
    }

    // Binary read methods
    char ReadByte() {
        if (!isOpen()) throwError(FILE_NOT_OPENED);
        switchToReadMode();

        if (bufferPosition >= bufferSize) {
            if (feof(file)) throwError(EOF_FOUND);
            fillBuffer();
            if (bufferSize == 0) throwError(EOF_FOUND);
        }

        char byte = buffer[bufferPosition];
        bufferPosition++;
        streamPosition++;
        return byte;
    }

    size_t ReadBlock(char* data, size_t size) {
        if (!isOpen()) throwError(FILE_NOT_OPENED);
        switchToReadMode();

        size_t totalRead = 0;

        while (size > 0) {
            if (bufferPosition >= bufferSize) {
                fillBuffer();
                if (bufferSize == 0) break; // EOF
            }

            size_t chunkSize = std::min(size, bufferSize - bufferPosition);
            std::memcpy(data + totalRead, buffer + bufferPosition, chunkSize);

            bufferPosition += chunkSize;
            totalRead += chunkSize;
            size -= chunkSize;
            streamPosition += chunkSize;
        }

        return totalRead;
    }

    // Binary write methods
    void WriteByte(const char byte) {
        if (!isOpen()) throwError(FILE_NOT_OPENED);
        switchToWriteMode();

        if (bufferPosition >= BUFFER_SIZE) {
            writeFromBuffer();
        }

        buffer[bufferPosition] = byte;
        bufferPosition++;
        streamPosition++;
    }

    size_t WriteBlock(const char* data, size_t size) {
        if (!isOpen()) throwError(FILE_NOT_OPENED);
        switchToWriteMode();

        size_t totalWritten = 0;

        while (size > 0) {
            size_t freeSpace = BUFFER_SIZE - bufferPosition;
            size_t chunkSize = std::min(size, freeSpace);

            std::memcpy(buffer + bufferPosition, data + totalWritten, chunkSize);
            bufferPosition += chunkSize;
            totalWritten += chunkSize;
            size -= chunkSize;
            streamPosition += chunkSize;

            if (bufferPosition >= BUFFER_SIZE) {
                writeFromBuffer();
            }
        }

        return totalWritten;
    }

    void Flush() {
        if (!isOpen()) throwError(FILE_NOT_OPENED);
        if (WriteMode) {
            flushWrite();
        }
    }

    [[nodiscard]] bool isEOF() const {
        if (!isOpen()) return true;
        if (WriteMode) return false;
        if (bufferPosition < bufferSize) return false;
        return feof(file);
    }

    [[nodiscard]] bool isWriteMode() const {
        return WriteMode;
    }
};

#endif