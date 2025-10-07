#ifndef ERR_H
#define ERR_H

typedef enum {
    MATRIX_OPERATION_OK = 0,
    END_OF_FILE_FOUND = 1,
    EXIT = 2,
    MEMORY_ALLOCATION_FAILED = 100,
    MATRIX_NOT_DEFINED = 200,
    OPERATION_NOT_DEFINED = 201,
    INCOMPATIBLE_MATRIX_TYPES = 300,
    INDEX_OUT_OF_BOUNDS = 3
} Err;

typedef struct {
    Err code;
    const char* message;
} ErrorMessage;

#endif