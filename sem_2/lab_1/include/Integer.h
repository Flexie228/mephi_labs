#ifndef INTEGER_H
#define INTEGER_H

#include <stdio.h>
#include "Err.h"
#include "TypeInfo.h"

static TypeInfo* INT_TYPE_INFO = NULL;

void intAdd(const void* arg1, const void* arg2, void* result);
void intMultiply(const void* arg1, const void* arg2, void* result);
void intMultipyByConst(void* arg, int multiplier);
void intZero(void* arg);
void intPrint(const void* arg);
Err intTake(void* arg);
void intRandom(void* arg);

TypeInfo* GetIntTypeInfo();

#endif