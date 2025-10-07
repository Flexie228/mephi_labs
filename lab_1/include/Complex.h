#ifndef COMPLEX_H
#define COMPLEX_H

#include <stdio.h>
#include "Err.h"
#include "TypeInfo.h"

static TypeInfo* COMPLEX_TYPE_INFO = NULL;

typedef struct Complex {
    double Re;
    double Im;
} Complex;

void complexAdd(const void* arg1, const void* arg2, void* result);
void complexMultiply(const void* arg1, const void* arg2, void* result);
void complexMultipyByConst(void* arg, int multiplier);
void complexZero(void* arg);
void complexPrint(const void* arg);
Err complexTake(void* arg);
void complexRandom(void* arg);

TypeInfo* GetComplexTypeInfo();

#endif