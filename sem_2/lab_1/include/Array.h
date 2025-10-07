#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>
#include "Err.h"
#include "Matrix.h"

Err ArrayInsert(Matrix **matrixArray, Matrix *matrix, size_t *matrixCount);
Err ArrayDelete(Matrix **matrixArray, size_t index, size_t *matrixCount);
Err printMatrixArray(Matrix** matrixArray, size_t* matrixCount);

#endif