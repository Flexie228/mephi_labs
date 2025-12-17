#ifndef EXCEPTIONS
#define EXCEPTIONS

#include <stdexcept>
#include <array>

struct Error {
    int code;
    const char* message;
};

enum ErrorCode {
    FILE_NOT_OPENED = 1,
    EOF_FOUND = 2,
    FILE_EMPTY = 3,
    FILE_OPEN_ERROR = 4,
    SEEK_ERROR = 5,
    FILE_READ_ERROR = 6,
    FILE_WRITE_ERROR = 7,
    ERRORS_COUNT
};

constexpr std::array<Error, ERRORS_COUNT> Errors = {{
    {FILE_NOT_OPENED, "Error: File is not opened.\n"},
    {EOF_FOUND, "EOF detected.\n"},
    {FILE_EMPTY, "File is empty.\n"},
    {FILE_OPEN_ERROR, "File open error occurred.\n"},
    {SEEK_ERROR, "Seek operation failed.\n"},
    {FILE_READ_ERROR, "File read error occurred.\n"},
    {FILE_WRITE_ERROR, "Write operation failed.\n"}
}};

inline void throwError(const ErrorCode code) {
    if (code >= ERRORS_COUNT) {
        throw std::runtime_error("Error: unknown error.");
    }
    throw std::runtime_error(Errors[code].message);
}

#endif