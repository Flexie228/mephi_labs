#ifndef ERRORS
#define ERRORS

#include <stdexcept>
#include <array>

struct Error {
    int code;
    const char* message;
};

enum ErrorCode {
    OK = 0,
    INDEX_OUT_OF_RANGE = 1,
    INVALID_ARGUMENT = 2,
    MEMORY_ALLOCATION_FAILED = 3,
    SEQUENCE_EMPTY = 4,
    DEQUE_EMPTY = 5,

    ERRORS_COUNT
};

const std::array<Error, ERRORS_COUNT> Errors = {{
    {OK, "Operation successfully ended"},
    {INDEX_OUT_OF_RANGE, "Error: Index is out of range"},
    {INVALID_ARGUMENT, "Error: Invalid argument"},
    {MEMORY_ALLOCATION_FAILED, "Error: Memory allocation failed"},
    {SEQUENCE_EMPTY, "Error: Sequence is empty"},
    {DEQUE_EMPTY, "Error: Deque is empty"}
}};

inline void throwError(ErrorCode code) {
    if (code < 0 || code >= ERRORS_COUNT) {
        throw std::runtime_error("Unknown error");
    }
    throw std::runtime_error(Errors[code].message);
}

#endif