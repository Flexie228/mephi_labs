#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Err.h"
#include "TypeInfo.h"
#include "CommonFuncs.h"
#include "Complex.h"

TypeInfo* GetComplexTypeInfo() {
    if (COMPLEX_TYPE_INFO == NULL) {
        COMPLEX_TYPE_INFO = (TypeInfo*)malloc(sizeof(TypeInfo));
        COMPLEX_TYPE_INFO->size = sizeof(Complex);
        COMPLEX_TYPE_INFO->add = complexAdd;
        COMPLEX_TYPE_INFO->multiply = complexMultiply;
        COMPLEX_TYPE_INFO->multiplyByConst = complexMultipyByConst;
        COMPLEX_TYPE_INFO->zero = complexZero;
        COMPLEX_TYPE_INFO->print = complexPrint;
        COMPLEX_TYPE_INFO->take = complexTake;
        COMPLEX_TYPE_INFO->random = complexRandom;
    }
    return COMPLEX_TYPE_INFO;
}

void complexAdd(const void* arg1, const void* arg2, void* result)
{
    ((Complex*)result)->Re = ((Complex*)arg1)->Re + ((Complex*)arg2)->Re;
    ((Complex*)result)->Im = ((Complex*)arg1)->Im + ((Complex*)arg2)->Im;
}

void complexMultiply(const void* arg1, const void* arg2, void* result)
{
    ((Complex*)result)->Re = (((Complex*)arg1)->Re * ((Complex*)arg2)->Re) - (((Complex*)arg1)->Im * ((Complex*)arg2)->Im);
    ((Complex*)result)->Im = (((Complex*)arg1)->Re * ((Complex*)arg2)->Im) + (((Complex*)arg2)->Re * ((Complex*)arg1)->Im);
}

void complexMultipyByConst(void* arg, int multiplier)
{
    ((Complex*)arg)->Re = ((Complex*)arg)->Re * (double)multiplier;
    ((Complex*)arg)->Im = ((Complex*)arg)->Im * (double)multiplier;
}

void complexZero(void* arg)
{
    ((Complex*)arg)->Re = 0.0;
    ((Complex*)arg)->Im = 0.0;
}

void complexPrint(const void* arg)
{
    printf("%.2lf%+.2lfi", ((const Complex*)arg)->Re, ((const Complex*)arg)->Im);
}

Err complexTake(void* arg)
{
    printf("Re: ");
    if (input_double(&(((Complex*)arg)->Re)) == END_OF_FILE_FOUND) return END_OF_FILE_FOUND;
    printf("Im: ");
    if (input_double(&(((Complex*)arg)->Im)) == END_OF_FILE_FOUND) return END_OF_FILE_FOUND;
    return MATRIX_OPERATION_OK;
}

void complexRandom(void* arg)
{
    double randomNumber = ((double)rand()/(double)(RAND_MAX)) * 20.0 - 10.0;
    ((Complex*)arg)->Re = randomNumber;
    randomNumber = ((double)rand()/(double)(RAND_MAX)) * 20.0 - 10.0;
    ((Complex*)arg)->Im = randomNumber;
}