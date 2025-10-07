#ifndef COMMONFUNCS_H
#define COMMONFUNCS_H

#include <stdio.h>
#include "Matrix.h"
#include "Err.h"

Err input_size_t(size_t* num);
Err input_size_t_twice(size_t* num1, size_t* num2);
Err input_int(int* num);
Err input_double(double* num);
Err input_action(size_t* action, size_t maxAction);

void printErrorMessage(Err operationResult);
void clearMemory(Matrix** matrixArray, size_t matrixCount);

#endif