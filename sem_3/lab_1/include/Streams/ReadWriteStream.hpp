#ifndef READ_WRITE_STREAM
#define READ_WRITE_STREAM

#include "Stream.hpp"
#include "../Exceptions.hpp"
#include <cstring>

class ReadWriteStream final : public Stream {
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
    explicit ReadWriteStream(const string& filename) : Stream(filename) {}
    ~ReadWriteStream() override = default;

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

    // Read methods
    char ReadChar() {
        if (!isOpen()) throwError(FILE_NOT_OPENED);
        switchToReadMode();

        if (bufferPosition >= bufferSize) {
            if (feof(file)) throwError(EOF_FOUND);
            fillBuffer();
            if (bufferSize == 0) throwError(EOF_FOUND);
        }

        char symbol = buffer[bufferPosition];
        bufferPosition++;
        streamPosition++;
        return symbol;
    }

    string ReadWord() {
        if (!isOpen()) throwError(FILE_NOT_OPENED);
        switchToReadMode();

        string result;

        // Skip delimiters
        while (true) {
            if (bufferPosition >= bufferSize) {
                fillBuffer();
                if (bufferSize == 0) return result;
            }

            char sym = buffer[bufferPosition];
            if (stopSymbols.find(sym) == string::npos) break;

            bufferPosition++;
            streamPosition++;
        }

        // Read word
        while (true) {
            if (bufferPosition >= bufferSize) {
                fillBuffer();
                if (bufferSize == 0) return result;
            }

            char ch = buffer[bufferPosition];
            if (stopSymbols.find(ch) != string::npos) break;

            result += ch;
            bufferPosition++;
            streamPosition++;
        }

        return result;
    }

    string ReadLine() {
        if (!isOpen()) throwError(FILE_NOT_OPENED);
        switchToReadMode();

        string result;

        while (true) {
            if (bufferPosition >= bufferSize) {
                fillBuffer();
                if (bufferSize == 0) break;
            }

            if (const auto* newlinePos = static_cast<char*>(std::memchr(buffer + bufferPosition, '\n', bufferSize - bufferPosition))) {
                size_t chunkSize = newlinePos - (buffer + bufferPosition);
                result.append(buffer + bufferPosition, chunkSize);
                bufferPosition += chunkSize + 1;
                streamPosition += chunkSize + 1;
                break;
            } else {
                result.append(buffer + bufferPosition, bufferSize - bufferPosition);
                streamPosition += bufferSize - bufferPosition;
                bufferPosition = bufferSize;
            }
        }

        return result;
    }

    string ReadTextFile() {
        if (!isOpen()) throwError(FILE_NOT_OPENED);
        switchToReadMode();

        long currentPos = ftell(file);
        if (currentPos == -1) throwError(SEEK_ERROR);

        if (fseek(file, 0, SEEK_END) != 0) throwError(SEEK_ERROR);
        long fileSize = ftell(file);
        if (fileSize == -1) throwError(SEEK_ERROR);
        if (fseek(file, 0, SEEK_SET) != 0) throwError(SEEK_ERROR);

        string result(fileSize, '\0');
        size_t bytesRead = fread(&result[0], sizeof(char), fileSize, file);
        result.resize(bytesRead);

        fseek(file, currentPos, SEEK_SET);
        return result;
    }

    // Write methods
    void WriteChar(const char sym) {
        if (!isOpen()) throwError(FILE_NOT_OPENED);
        switchToWriteMode();

        if (bufferPosition >= BUFFER_SIZE) {
            writeFromBuffer();
        }

        buffer[bufferPosition] = sym;
        bufferPosition++;
        streamPosition++;
    }

    void WriteString(const string& data) {
        if (!isOpen()) throwError(FILE_NOT_OPENED);
        switchToWriteMode();

        for (size_t i = 0; i < data.length(); i++) {
            if (bufferPosition >= BUFFER_SIZE) {
                writeFromBuffer();
            }
            buffer[bufferPosition] = data[i];
            bufferPosition++;
        }
        streamPosition += data.length();
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