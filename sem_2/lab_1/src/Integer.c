#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "CommonFuncs.h"
#include "Err.h"
#include "TypeInfo.h"
#include "Integer.h"

TypeInfo* GetIntTypeInfo() {
    if (INT_TYPE_INFO == NULL) {
        INT_TYPE_INFO = (TypeInfo*)malloc(sizeof(TypeInfo));
        INT_TYPE_INFO->size = sizeof(int);
        INT_TYPE_INFO->add = intAdd;
        INT_TYPE_INFO->multiply = intMultiply;
        INT_TYPE_INFO->multiplyByConst = intMultipyByConst;
        INT_TYPE_INFO->zero = intZero;
        INT_TYPE_INFO->print = intPrint;
        INT_TYPE_INFO->take = intTake;
        INT_TYPE_INFO->random = intRandom;
    }
    return INT_TYPE_INFO;
}

void intAdd(const void* arg1, const void* arg2, void* result)
{
    *(int*)result = *(int*)arg1 + *(int*)arg2;
}

void intMultiply(const void* arg1, const void* arg2, void* result)
{
    *(int*)result = *(int*)arg1 * *(int*)arg2;
}

void intMultipyByConst(void* arg, int multiplier)
{
    *(int*)arg = multiplier * *(int*)arg;
}

void intZero(void* arg)
{
    *(int*)arg = 0;
}

void intPrint(const void* arg)
{
    printf("%d", *(const int*)arg);
}

Err intTake(void* arg)
{
    if (input_int((int*)arg) == END_OF_FILE_FOUND) return END_OF_FILE_FOUND;
    return MATRIX_OPERATION_OK;
}

void intRandom(void* arg)
{
    int randomNumber = rand() % 21 - 10;
    *(int*)arg = randomNumber;
}