#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include "Err.h"
#include "TypeInfo.h"

typedef struct {
    size_t rows;
    size_t cols;
    void* data;
    TypeInfo* typeInfo;
} Matrix;

Matrix* createMatrix(TypeInfo* typeInfo, size_t rows, size_t cols, Err* OperationResult);

Err addMatrix(const Matrix* m1, const Matrix* m2, Matrix* res);
Err multiplyMatrix(const Matrix* m1, const Matrix* m2, Matrix* res);
Err multiplyMatrixByConst(Matrix* matrix);
Err changeMatrix(Matrix* matrix, size_t row, size_t col);
Err lineCombinationMatrix(Matrix* matrix);
Err transposeMatrix(const Matrix* m, Matrix* res);
Err fillMatrix(Matrix* matrix);
void fillMatrixByRandom(Matrix* matrix);

void* getElement(const Matrix* m, size_t row, size_t col);
void setElement(Matrix* m, size_t row, size_t col,const void* value);
void multiplyRow(Matrix* m, int multiplier, size_t rowNumber);
void multiplyCol(Matrix* matrix, int multiplier, size_t colNumber);
void printMatrix(const Matrix* m);
void freeMatrix(Matrix* m);

#endif