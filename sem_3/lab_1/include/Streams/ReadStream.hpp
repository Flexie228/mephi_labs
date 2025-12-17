#ifndef READ_STREAM
#define READ_STREAM

#include <cstring>
#include "../Exceptions.hpp"
#include "Stream.hpp"

class ReadStream final : public Stream {
private:
    void fillBuffer() {
        if (!file) return;
        bufferSize = fread(buffer, sizeof(char), BUFFER_SIZE, file);
        bufferPosition = 0;
    }
public:
    explicit ReadStream(const string& filename) : Stream(filename) {}
    ~ReadStream() override = default;

    void Open() override {
        if (file) return;
        if (filename.empty()) throwError(FILE_EMPTY);

        file = fopen(filename.c_str(), "rb");
        if (!file) throwError(FILE_OPEN_ERROR);

        OpenFlag = true;
        streamPosition = 0;
        WriteMode = false;
        fillBuffer();
    }
    void Close() override {
        if (!isOpen()) return;

        fclose(file);
        file = nullptr;
        OpenFlag = false;
        bufferPosition = 0;
        bufferSize = 0;
        streamPosition = 0;
    }
    size_t Seek(const size_t pos) override {
        if (!file || !OpenFlag) throwError(FILE_NOT_OPENED);
        if (static_cast<long>(pos) > getSize()) throwError(SEEK_ERROR);
        int result = fseek(file, static_cast<long>(pos), SEEK_SET);
        if (result != 0) throwError(SEEK_ERROR);
        streamPosition = ftell(file);
        fillBuffer();
        return streamPosition;
    }
    char ReadChar() {
        if (!file || !OpenFlag) throwError(FILE_NOT_OPENED);

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
        if (!file || !OpenFlag) throwError(FILE_NOT_OPENED);
        string result;

        do {
            if (bufferPosition >= bufferSize) {
                fillBuffer();
                if (bufferSize == 0) return result; // конец файла
            }

            char sym = buffer[bufferPosition];
            if (stopSymbols.find(sym) == string::npos) break; // нашли не-разделитель

            bufferPosition++;
            streamPosition++;
        } while (bufferSize != 0);

        // Читаем слово
        do {
            if (bufferPosition >= bufferSize) {
                fillBuffer();
                if (bufferSize == 0) return result; // конец файла
            }

            char ch = buffer[bufferPosition];
            if (stopSymbols.find(ch) != string::npos) break; // нашли разделитель

            result += ch;
            bufferPosition++;
            streamPosition++;
        } while (bufferSize != 0);

        return result;
    }
    string ReadLine() {
        if (!file || !OpenFlag) throwError(FILE_NOT_OPENED);
        string result;
        do {
            // Если буфер пуст - заполняем
            if (bufferPosition >= bufferSize) {
                fillBuffer();
                if (bufferSize == 0) break; // конец файла
            }

            // Ищем '\n' в текущем буфере
            if (const auto* newlinePos = static_cast<char*>(std::memchr(buffer + bufferPosition, '\n', bufferSize - bufferPosition))) {
                size_t chunkSize = newlinePos - (buffer + bufferPosition);
                result.append(buffer + bufferPosition, chunkSize);
                bufferPosition += chunkSize + 1; // +1 чтобы пропустить '\n'
                streamPosition += chunkSize + 1;
                break;
            } else {
                result.append(buffer + bufferPosition, bufferSize - bufferPosition);
                streamPosition += bufferSize - bufferPosition;
                bufferPosition = bufferSize; // буфер пуст
            }
        } while (bufferSize != 0);
        return result;
    }
    // ReSharper disable once CppMemberFunctionMayBeConst
    string ReadTextFile() {
        if (!file || !OpenFlag) throwError(FILE_NOT_OPENED);

        // Сохраняем текущую позицию
        long currentPos = ftell(file);
        if (currentPos == -1) throwError(SEEK_ERROR);

        if (fseek(file, 0, SEEK_END) != 0) throwError(SEEK_ERROR);
        long fileSize = ftell(file);
        if (fileSize == -1) throwError(SEEK_ERROR);
        if (fseek(file, 0, SEEK_SET) != 0) throwError(SEEK_ERROR);

        // Читаем ВЕСЬ файл за один вызов
        string result(fileSize, '\0');
        size_t bytesRead = fread(&result[0], sizeof(char), fileSize, file);
        result.resize(bytesRead);

        // Восстанавливаем позицию
        fseek(file, currentPos, SEEK_SET);
        return result;
    }
    [[nodiscard]] bool isEOF() const {
        if (!file || !OpenFlag) return true;
        if (bufferPosition < bufferSize) return false;

        return feof(file);
    }
};




#endif