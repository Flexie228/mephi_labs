#ifndef DOUBLE_H
#define DOUBLE_H

#include <stdio.h>
#include "TypeInfo.h"
#include "Err.h"

static TypeInfo* DOUBLE_TYPE_INFO = NULL;

void doubleAdd(const void* arg1, const void* arg2, void* result);
void doubleMultiply(const void* arg1, const void* arg2, void* result);
void doubleMultipyByConst(void* arg, int multiplier);
void doubleZero(void* arg);
void doublePrint(const void* arg);
Err doubleTake(void* arg);
void doubleRandom(void* arg);

TypeInfo* GetDoubleTypeInfo();

#endif