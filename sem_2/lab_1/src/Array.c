#include <stdio.h>
#include <stdlib.h>
#include "Err.h"
#include "Matrix.h"
#include "CommonFuncs.h"

Err ArrayInsert(Matrix **matrixArray, Matrix *matrix, size_t *matrixCount) {
    Matrix *newMatrixArray = (Matrix*)realloc(*matrixArray, sizeof(Matrix) * ((*matrixCount) + 1));
    if (newMatrixArray == NULL) {
        return MEMORY_ALLOCATION_FAILED;
    }
    *matrixArray = newMatrixArray;
    (*matrixArray)[*matrixCount] = *matrix; 
    (*matrixCount)++;
    return MATRIX_OPERATION_OK;
}

Err ArrayDelete(Matrix **matrixArray, size_t index, size_t *matrixCount)
{
    if (index >= *matrixCount) return INDEX_OUT_OF_BOUNDS;
    freeMatrix(&(*matrixArray)[index]);
    while (index < *matrixCount - 1) {
        (*matrixArray)[index] = (*matrixArray)[index + 1];
        index++;
    }
    (*matrixCount)--;

    if (*matrixCount == 0) {
        free(*matrixArray);
        *matrixArray = NULL;
        return MATRIX_OPERATION_OK;
    }

    Matrix *allocArray = realloc(*matrixArray, sizeof(Matrix) * (*matrixCount));
    if (allocArray == NULL && *matrixCount > 0) return MEMORY_ALLOCATION_FAILED;
    *matrixArray = allocArray;

    return MATRIX_OPERATION_OK;
}

Err printMatrixArray(Matrix** matrixArray, size_t* matrixCount) {
    if (matrixArray == NULL || *matrixCount == 0) {
        printErrorMessage(MATRIX_NOT_DEFINED);
        return MATRIX_NOT_DEFINED;
    }

    for (size_t i = 0; i < *matrixCount; i++) {
        printf("Matrix %zu, %zux%zu:\n", (i+1), (*matrixArray)[i].rows, (*matrixArray)[i].cols);
        printMatrix(&((*matrixArray)[i]));
        printf("\n");
    }
    return MATRIX_OPERATION_OK;
}