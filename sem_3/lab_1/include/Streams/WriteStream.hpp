#ifndef WRITESTREAM_HPP
#define WRITESTREAM_HPP

#include <cstring>

#include "../Stream.hpp"
#include "../Exceptions.hpp"

class WriteStream final : public Stream {
private:
    void writeFromBuffer() {
        if (bufferPosition > 0 && file) {
            size_t written = fwrite(buffer, sizeof(char), bufferPosition, file);
            if (written != static_cast<size_t>(bufferPosition)) throwError(FILE_WRITE_ERROR);
            bufferPosition = 0;
        }
    }
    void flushBuffer() {
        streamPosition += bufferPosition;
        writeFromBuffer();
        if (file) {
            fflush(file);
        }
    }
public:
    explicit WriteStream(const string& filename) : Stream(filename){}
    ~WriteStream() override = default;

    void Open() override {
        if (isOpen()) return;
        if (filename.empty()) throwError(FILE_EMPTY);
        file = fopen(filename.c_str(), "wb");
        if (!file) throwError(FILE_OPEN_ERROR);
        OpenFlag = true;
        streamPosition = 0;
        bufferPosition = 0;
        WriteMode = true;
    }
    void Close() override {
        if (!isOpen()) return;

        writeFromBuffer();
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

        writeFromBuffer();

        int result = fseek(file, static_cast<long>(pos), SEEK_SET);
        if (result != 0) throwError(SEEK_ERROR);

        streamPosition = ftell(file);
        return streamPosition;
    }
    void WriteChar(const char sym) {
        if (!isOpen()) throwError(FILE_NOT_OPENED);

        if (bufferPosition >= BUFFER_SIZE) {
            writeFromBuffer();
        }

        buffer[bufferPosition] = sym;
        bufferPosition++;
        streamPosition++;
    }

    void WriteString(const string& data) {
        if (!isOpen()) throwError(FILE_NOT_OPENED);

        size_t dataPos = 0;
        size_t dataSize = data.length();

        while (dataPos < dataSize) {
            size_t freeSpace = BUFFER_SIZE - bufferPosition;
            size_t chunkSize = std::min(freeSpace, dataSize - dataPos);
            std::memcpy(buffer + bufferPosition, data.data() + dataPos, chunkSize);
            bufferPosition += chunkSize;
            dataPos += chunkSize;

            if (bufferPosition >= BUFFER_SIZE) {
                writeFromBuffer();
            }
        }

        streamPosition += data.length();
    }
    void Flush() {
        if (!isOpen()) throwError(FILE_NOT_OPENED);
        flushBuffer();
    }
};


#endif