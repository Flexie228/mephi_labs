#ifndef TYPEINFO_H
#define TYPEINFO_H

#include <stdio.h>
#include "Err.h"

typedef void (*BinaryOperator)(const void* arg1, const void* arg2, void* result);

typedef struct {
    size_t size;
    BinaryOperator add;
    BinaryOperator multiply;
    void (*print)(const void*);
    void (*zero)(void*);
    void (*multiplyByConst)(void*, int multiplier);
    Err (*take)(void*);
    void (*random)(void*);
} TypeInfo;

#endif