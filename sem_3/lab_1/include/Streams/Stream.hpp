#ifndef STREAM_HPP
#define STREAM_HPP

#include <string>
#include <utility>
using namespace std;

inline string stopSymbols = "\t\n\r,.!?;:'\"()- ";

class Stream {
protected:
    string filename;
    FILE* file{nullptr};

    bool OpenFlag{false};
    bool WriteMode{false};
    size_t streamPosition{0};

    static constexpr int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE]{};
    size_t bufferPosition{0};
    size_t bufferSize{0};

public:
    explicit Stream(string  filename = "") : filename(std::move(filename)) {};
    virtual ~Stream() = default;

    virtual void Open() = 0;
    virtual void Close() = 0;
    virtual size_t Seek(size_t i) = 0;

    [[nodiscard]] bool isOpen() const { return OpenFlag; }
    [[nodiscard]] bool isWrite() const { return WriteMode; }
    [[nodiscard]] long getSize() const {
        if (!isOpen()) return 0;

        long currentPosition = ftell(file);
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, currentPosition, SEEK_SET);
        return fileSize;
    }
    [[nodiscard]] string getFilename() const { return filename; }

};

#endif