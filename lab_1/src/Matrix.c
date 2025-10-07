#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Matrix.h"
#include "CommonFuncs.h"

Matrix* createMatrix(TypeInfo* typeInfo, size_t rows, size_t cols, Err* operationResult)
{
    Matrix* matrix = malloc(sizeof(Matrix));
    if (matrix == NULL) {
        *operationResult = MEMORY_ALLOCATION_FAILED;
        return NULL;
    }
    
    matrix->typeInfo = typeInfo;
    matrix->rows = rows;
    matrix->cols = cols;
    
    matrix->data = malloc(rows * cols * (typeInfo->size));
    if (matrix->data == NULL) {
        *operationResult = MEMORY_ALLOCATION_FAILED;
        return NULL;
    }
    
    *operationResult = MATRIX_OPERATION_OK;
    return matrix;
}

Err addMatrix(const Matrix* matrix_1, const Matrix* matrix_2, Matrix* res)
{
    if (matrix_1 == NULL || matrix_2 == NULL) return MATRIX_NOT_DEFINED;
    if (matrix_1->typeInfo->size != matrix_2->typeInfo->size || matrix_1->rows != matrix_2->rows || matrix_1->cols != matrix_2->cols) return INCOMPATIBLE_MATRIX_TYPES;

    res->typeInfo = matrix_1->typeInfo;
    res->rows = matrix_1->rows;
    res->cols = matrix_1->cols;
    res->data = malloc((res->typeInfo->size) * (res->rows) * (res->cols));
    if (res->data == NULL) return MEMORY_ALLOCATION_FAILED;

    for (size_t i = 0; i < matrix_1->rows; i++) {
        for (size_t j = 0; j < matrix_1->cols; j++) {
            void* arg1 = getElement(matrix_1, i, j);
            void* arg2 = getElement(matrix_2, i, j);
            void* value = getElement(res, i, j);
            matrix_1->typeInfo->add(arg1, arg2, value);
        }
    }
    return MATRIX_OPERATION_OK;
}

Err multiplyMatrix(const Matrix* matrix_1, const Matrix* matrix_2, Matrix* res)
{
    if (matrix_1 == NULL || matrix_2 == NULL) return MATRIX_NOT_DEFINED;
    if (res == NULL) return MEMORY_ALLOCATION_FAILED;
    if (matrix_1->typeInfo->size != matrix_2->typeInfo->size || matrix_1->cols != matrix_2->rows) return INCOMPATIBLE_MATRIX_TYPES;

    res->typeInfo = matrix_1->typeInfo;
    res->rows = matrix_1->rows;
    res->cols = matrix_2->cols;
    res->data = malloc((res->typeInfo->size) * (res->rows) * (res->cols));
    if (res->data == NULL) return MEMORY_ALLOCATION_FAILED;

    void* intermidValue = malloc(res->typeInfo->size);      
    if (intermidValue == NULL) return MEMORY_ALLOCATION_FAILED;  

    for (size_t i = 0; i < matrix_1->rows; i++) {
        for (size_t j = 0; j < matrix_2->cols; j++) {
            void* resultValue = getElement(res, i, j);
            res->typeInfo->zero(resultValue); 
            for (size_t k = 0; k < matrix_1->cols; k++) {
                void* arg1 = getElement(matrix_1, i, k);
                void* arg2 = getElement(matrix_2, k, j);
                res->typeInfo->multiply(arg1, arg2, intermidValue);
                res->typeInfo->add(resultValue, intermidValue, resultValue);
            }
        }
    }
    return MATRIX_OPERATION_OK;
}

Err multiplyMatrixByConst(Matrix* matrix)
{
    size_t action;
    printf("Умножить...\n");
    printf("1) Строку\n");
    printf("2) Столбец\n");
    printf("Выбор: ");
    if (input_action(&action, 2) == END_OF_FILE_FOUND) return END_OF_FILE_FOUND;

    int multiplier;
    printf("Введите число, на которое умножаем: ");
    if (input_int(&multiplier) == END_OF_FILE_FOUND) return END_OF_FILE_FOUND;

    switch(action) {
        case 1:
            printf("Введите номер строки: ");
            if (input_action(&action, matrix->rows) == END_OF_FILE_FOUND) return END_OF_FILE_FOUND;
            multiplyRow(matrix, multiplier, action);
            break;
        case 2:
            printf("Введите номер столбца: ");
            if (input_action(&action, matrix->cols) == END_OF_FILE_FOUND) return END_OF_FILE_FOUND;
            multiplyCol(matrix, multiplier, action);
            break;
    }

}

void multiplyRow(Matrix* matrix, int multiplier, size_t rowNumber)
{
    for (size_t col = 0; col < matrix->cols; col++) {
        void* elementPtr = getElement(matrix, rowNumber, col);
        matrix->typeInfo->multiplyByConst(elementPtr, multiplier);
    }
}

void multiplyCol(Matrix* matrix, int multiplier, size_t colNumber) {
    for (size_t row = 0; row < matrix->rows; row++) {
        void* elementPtr = getElement(matrix, row, colNumber);
        matrix->typeInfo->multiplyByConst(elementPtr, multiplier);
    }
}

Err fillMatrix(Matrix* matrix)
{
    size_t elementSize = matrix->typeInfo->size;
    for (size_t i = 0; i < matrix->rows; i++) {
        for (size_t j = 0; j < matrix->cols; j++) {
            printf("Элемент с индексом %zu %zu: ", i + 1, j + 1);
            void* elementPtr = (char*)matrix->data + (i * matrix->cols + j) * elementSize;
            matrix->typeInfo->take(elementPtr);
        }
    }
    return MATRIX_OPERATION_OK;
}

void fillMatrixByRandom(Matrix* matrix)
{
    size_t elementSize = matrix->typeInfo->size;
    for (size_t i = 0; i < matrix->rows; i++) {
        for (size_t j = 0; j < matrix->cols; j++) {
            void* elementPtr = (char*)matrix->data + (i * matrix->cols + j) * elementSize;
            matrix->typeInfo->random(elementPtr);
        }
    }        
}

Err changeMatrix(Matrix* matrix, size_t row, size_t col)
{
    if (row >= matrix->rows || col >= matrix->cols) return INDEX_OUT_OF_BOUNDS;
    
    size_t elementSize = matrix->typeInfo->size;
    void* elementPtr = (char*)matrix->data + (row * matrix->cols + col) * elementSize;

    printf("Введите новое значение элемента %zu %zu: ", row, col + 1);
    return matrix->typeInfo->take(elementPtr);
}

Err lineCombinationMatrix(Matrix* matrix)
{
    size_t action, num1, num2;
    printf("Идем по...\n");
    printf("1) строкам\n");
    printf("2) столбцам\n");
    printf("Действие: \n");
    if (input_action(&action, 2) == END_OF_FILE_FOUND) return END_OF_FILE_FOUND;
    size_t flag = 0;
    do  {
        flag = 0;
        printf("Введите 2 числа!\n");
        printf("1) что прибавляем\n");
        printf("2) к чему прибавляем\n");
        printf("Выбор: \n");
        if (input_size_t_twice(&num1, &num2) == END_OF_FILE_FOUND) return END_OF_FILE_FOUND;
        if ((action == 1) && (num1 > matrix->rows || num2 > matrix->rows)) {
            printf("Ошибка ввода. Таких строк не существует!\n");
            flag = 1;
        }
        if ((action == 2) && (num1 > matrix->cols || num2 > matrix->cols)) {
            printf("Ошибка ввода. Таких столбцов не существует!\n");
            flag = 1;
        }
    } while (flag != 0);

    int multiplier;
    if (action == 1) printf("Введите целое число, на которое умножаем строку %zu, чтобы прибавить ее к строке %zu: ", num1, num2);
    if (action == 2) printf("Введите целое число, на которое умножаем столбец %zu, чтобы прибавить его к столбцу %zu: ", num1, num2);
    if (input_int(&multiplier) == END_OF_FILE_FOUND) return END_OF_FILE_FOUND;

    num1--;
    num2--;
    if (action == 1) {
        for (size_t i = 0; i < matrix->cols; i++) {
            void* multiplierElement = malloc(matrix->typeInfo->size);
            if (multiplierElement == NULL) return MEMORY_ALLOCATION_FAILED;
            void* element1 = (char*)matrix->data + (num1 * matrix->cols + i) * matrix->typeInfo->size;
            void* element2 = (char*)matrix->data + (num2 * matrix->cols + i) * matrix->typeInfo->size;
            memcpy(multiplierElement, element1, matrix->typeInfo->size);
            matrix->typeInfo->multiplyByConst(multiplierElement, multiplier);
            matrix->typeInfo->add(multiplierElement, element2, element2);
            free(multiplierElement);
        }
    } else {
        for (size_t i = 0; i < matrix->rows; i++) {
            void* multiplierElement = malloc(matrix->typeInfo->size);
            if (multiplierElement == NULL) return MEMORY_ALLOCATION_FAILED;
            void* element1 = (char*)matrix->data + (i * matrix->cols + num1) * matrix->typeInfo->size;
            void* element2 = (char*)matrix->data + (i * matrix->cols + num2) * matrix->typeInfo->size;
            memcpy(multiplierElement, element1, matrix->typeInfo->size);
            matrix->typeInfo->multiplyByConst(multiplierElement, multiplier);
            matrix->typeInfo->add(multiplierElement, element2, element2);
            free(multiplierElement);
        }
    }
    return MATRIX_OPERATION_OK;
}

Err transposeMatrix(const Matrix* matrix, Matrix* result)
{
    if (matrix == NULL) return MATRIX_NOT_DEFINED;
    if (matrix == result) return OPERATION_NOT_DEFINED;

    result->typeInfo = matrix->typeInfo;
    result->rows = matrix->cols;
    result->cols = matrix->rows;
    result->data = malloc(result->rows * result->cols * (result->typeInfo->size));
    if (result->data == NULL) return MEMORY_ALLOCATION_FAILED;

    for (size_t i = 0; i < matrix->rows; i++) {
        for (size_t j = 0; j < matrix->cols; j++) {
            void* value = getElement(matrix, i, j);
            setElement(result, j, i, value);
        }
    }
    return MATRIX_OPERATION_OK;
}



void printMatrix(const Matrix* matrix) {
    if (matrix == NULL) {
        printf("NULL MATRIX PTR\n");
        return;
    }
    if (matrix->typeInfo == NULL) {
        printf("TypeInfo не определен\n");
        return;
    }
    printf("| ");
    for (size_t row = 0; row < matrix->rows; row++) {
        for (size_t col = 0; col < matrix->cols; col++) {
            void* elementPtr = (char*)matrix->data + (row * matrix->cols + col) * matrix->typeInfo->size;
            matrix->typeInfo->print(elementPtr);
            printf(" ");
        }
        if (row < (matrix->rows - 1)) printf("\n| ");
    }
    printf("\n");
}

void setElement(Matrix* m, size_t row, size_t col,const void* value)
{
    size_t elementSize = m->typeInfo->size;
    size_t offset = (row * (m->cols) + col) * elementSize;
    memcpy((char*)m->data + offset, value, elementSize);
    return;
}

void* getElement(const Matrix* matrix, size_t row, size_t col)
{
    size_t elementSize = matrix->typeInfo->size;
    size_t offset =((row * (matrix->cols) + col) * elementSize);
    return((char*)matrix->data + offset);
}

void freeMatrix(Matrix* matrix)
{
    if (matrix->data != NULL) {
        free(matrix->data);
        matrix->data = NULL;
    }
}