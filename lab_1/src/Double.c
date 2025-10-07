#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Typeinfo.h"
#include "CommonFuncs.h"
#include "Double.h"
#include "Err.h"

TypeInfo* GetDoubleTypeInfo() {
    if (DOUBLE_TYPE_INFO == NULL) {
        DOUBLE_TYPE_INFO = (TypeInfo*)malloc(sizeof(TypeInfo));
        DOUBLE_TYPE_INFO->size = sizeof(double);
        DOUBLE_TYPE_INFO->add = doubleAdd;
        DOUBLE_TYPE_INFO->multiplyByConst = doubleMultipyByConst;
        DOUBLE_TYPE_INFO->multiply = doubleMultiply;
        DOUBLE_TYPE_INFO->zero = doubleZero;
        DOUBLE_TYPE_INFO->print = doublePrint;
        DOUBLE_TYPE_INFO->take = doubleTake;
        DOUBLE_TYPE_INFO->random = doubleRandom;
    }
    return DOUBLE_TYPE_INFO;
}

void doubleAdd(const void* arg1, const void* arg2, void* result)
{
    *(double*)result = *(double*)arg1 + *(double*)arg2;
}

void doubleMultiply(const void* arg1, const void* arg2, void* result)
{
    *(double*)result = *(double*)arg1 * *(double*)arg2;
}

void doubleMultipyByConst(void* arg, int multiplier)
{
    *(double*)arg = (double)multiplier * *(double*)arg;
}

void doubleZero(void* arg)
{
    *(double*)arg = 0.0;
}

void doublePrint(const void* arg)
{
    printf("%.2lf", *(const double*)arg);
}

Err doubleTake(void* arg)
{
    if (input_double((double*)arg) == END_OF_FILE_FOUND) return END_OF_FILE_FOUND;
    return MATRIX_OPERATION_OK;
}

void doubleRandom(void* arg)
{
    double randomNumber = ((double)rand()/(double)(RAND_MAX)) * 20.0 - 10.0;
    *(double*)arg = randomNumber;
}