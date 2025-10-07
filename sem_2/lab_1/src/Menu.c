#include <stdio.h>
#include <stdlib.h>
#include "Menu.h"
#include "Err.h"
#include "CommonFuncs.h"
#include "Integer.h"
#include "Double.h"
#include "Complex.h"
#include "Array.h"
#include "Matrix.h"

void enterProgramm()
{
    Err operationResult = MATRIX_OPERATION_OK;
    Matrix* matrixArray = NULL;
    size_t matrixCount = 0;

    do {


        size_t action;
        printf("1) Действия над матрицами.\n");
        printf("2) Создать матрицу.\n");
        printf("3) Изменить матрицу.\n");
        printf("4) Удалить матрицу.\n");
        printf("5) Вывести матрицу(-ы) на экран.\n");
        printf("6) Выйти.\n");
        printf("Выберите действие: ");
        input_action(&action, 6);
        switch (action)
        {
        case 1:
            actionMenu(&matrixArray, &matrixCount, &operationResult);
            break;
        case 2:
            createMenu(&matrixArray, &matrixCount, &operationResult);
            break;
        case 3:
            changeMenu(&matrixArray, matrixCount, &operationResult);
            break;
        case 4:
            deleteMenu(&matrixArray, &matrixCount, &operationResult);
            break;
        case 5:
            operationResult = printMatrixArray(&matrixArray, &matrixCount);
            break;
        case 6:
            operationResult = EXIT;
            return;
        default:
            break;
        }
    } while (operationResult != END_OF_FILE_FOUND || operationResult != MEMORY_ALLOCATION_FAILED || operationResult != EXIT);
    clearMemory(&matrixArray, matrixCount);
}

void actionMenu(Matrix** matrixArray, size_t* matrixCount, Err* operationResult)
{
    size_t action;
    if (*matrixCount < 1) {
        *operationResult = MATRIX_NOT_DEFINED;
        system("cls");
        printErrorMessage(*operationResult);
        return;
    }

    printf("1) Сложение матриц.\n");
    printf("2) Умножение матриц.\n");
    printf("3) Прибаление к строке(столбцу) линейной комбинации других строк(столбцов).\n");
    printf("4) Умножение на константу строки(столбца).\n");
    printf("5) Транспонирование.\n");
    printf("Выберите действие: ");
    if ((*operationResult = input_action(&action, 5)) == END_OF_FILE_FOUND) return;
    
    Matrix matrix;
    size_t m1;
    if (action == 1 || action == 2) {
        size_t m2;
        printf("Выберите номера матриц, над которыми хотите произвести операцию.\n");
        printf("Помните, что операция сложения матриц некоммутативна, следовательно, первое число - номер левой матрицы, правое - правой.\n");
        printf("Например: 1 4.\n");
        printf("Введите номера матриц: ");
        if((*operationResult = input_size_t_twice(&m1, &m2)) == END_OF_FILE_FOUND) return;

        if (m1 > *matrixCount || m2 > *matrixCount) {
            *operationResult = MATRIX_NOT_DEFINED;
            printErrorMessage(*operationResult);
            return;
        }

        switch (action) {
            case 1:
                *operationResult = addMatrix(&((*matrixArray)[m1-1]), &((*matrixArray)[m2-1]), &matrix);
                break;
            case 2:
                *operationResult = multiplyMatrix(&((*matrixArray)[m1-1]), &((*matrixArray)[m2-1]), &matrix);
                break;
        }
        if (*operationResult == MATRIX_OPERATION_OK) *operationResult = ArrayInsert(matrixArray, &matrix, matrixCount); 
    } else {
        printf("Введите номер матрицы: ");
        if (*operationResult = input_action(&m1, *matrixCount) == END_OF_FILE_FOUND) return;

        switch (action) {
            case 3:
                *operationResult = lineCombinationMatrix(&((*matrixArray)[m1-1]));
                break;
            case 4:
                *operationResult = multiplyMatrixByConst(&((*matrixArray)[m1-1]));
                break;
            case 5:
                *operationResult = transposeMatrix(&((*matrixArray)[m1-1]), &matrix);
                if (*operationResult == MATRIX_OPERATION_OK) *operationResult = ArrayInsert(matrixArray, &matrix, matrixCount);
                break;
        }
    }
    system("cls");
    printErrorMessage(*operationResult);
    if (*operationResult == MATRIX_OPERATION_OK) {
        printf("Добавлена матрица %zux%zu:\n", matrix.rows, matrix.cols);
        printMatrix(&matrix);
    }
}

void createMenu(Matrix** matrixArray, size_t* matrixCount, Err* operationResult)
{
    Matrix matrix;
    size_t action, rows, cols;

    printf("1) Целочисленный.\n");
    printf("2) Вещественный.\n");
    printf("3) Комплексный.\n");
    printf("Выберите тип: ");
    *operationResult = input_action(&action, 3);
    if (*operationResult == END_OF_FILE_FOUND) return;

    getRowsAndColsMenu(&rows, &cols, operationResult);
    if (*operationResult == END_OF_FILE_FOUND) return;

    switch (action) {
        case 1:
            matrix = *createMatrix(GetIntTypeInfo(), rows, cols, operationResult);
            break;
        case 2:
            matrix = *createMatrix(GetDoubleTypeInfo(), rows, cols, operationResult);
            break;
        case 3:
            matrix = *createMatrix(GetComplexTypeInfo(), rows, cols, operationResult);
            break;
        default:
            return;
    }
    if (*operationResult == MATRIX_OPERATION_OK) *operationResult = ArrayInsert(matrixArray, &matrix, matrixCount);
    if (*operationResult == MATRIX_OPERATION_OK) fillMenu((&matrix), operationResult);
    system("cls");
    printErrorMessage(*operationResult);
    if (*operationResult == MATRIX_OPERATION_OK) {
        printf("Добавлена матрица %zux%zu:\n", matrix.rows, matrix.cols);
        printMatrix(&matrix);
    }
}

void changeMenu(Matrix** matrixArray, size_t matrixCount, Err* operationResult)
{
    if (matrixCount < 1) {
        *operationResult = MATRIX_NOT_DEFINED;
        system("cls");
        printErrorMessage(*operationResult);
        return;
    }
    printf("Введите номер матрицы которую хотите изменить.\n");
    printf("Номер: ");
    size_t number, row, col;
    if (*operationResult = input_action(&number, matrixCount) == END_OF_FILE_FOUND) return;

    printf("Введите строку, в которой находится элемент: ");
    if (*operationResult = input_action(&row, ((*matrixArray)[number-1]).rows) == END_OF_FILE_FOUND) return;

    printf("Введите столбец, в которой находится элемент: ");
    if (*operationResult = input_action(&col, ((*matrixArray)[number-1]).cols) == END_OF_FILE_FOUND) return;

    *operationResult = changeMatrix(&((*matrixArray)[number-1]), row-1, col-1);
    system("cls");
    printErrorMessage(*operationResult);
    if (*operationResult == MATRIX_OPERATION_OK) {
        printf("Изменена матрица %zux%zu:\n", (*matrixArray)[number-1].rows, (*matrixArray)[number-1].cols);
        printMatrix(&((*matrixArray)[number-1]));
    }
}

void deleteMenu(Matrix** matrixArray, size_t* matrixCount, Err* operationResult)
{
    if (*matrixCount < 1) {
        *operationResult = MATRIX_NOT_DEFINED;
        system("cls");
        printErrorMessage(*operationResult);
        return;
    }
    size_t action;
    printf("Введите номер матрицы, которую хотите удалить.\n");
    printf("Если вы не хотите ничего удалять, введите %zu.\n", *matrixCount+1);
    printf("Номер: ");
    *operationResult = input_action(&action, *matrixCount+1);
    if (*operationResult == END_OF_FILE_FOUND || action == *matrixCount+1) return;
    if ((*operationResult = ArrayDelete(matrixArray, action - 1, matrixCount)) == MATRIX_OPERATION_OK) printf("Матрица %zu успешно удалена!\n", action);
    printErrorMessage(*operationResult);
}

void fillMenu(Matrix* matrix, Err* operationResult)
{
    printf("Заполнить...\n");
    printf("1) Вручную\n");
    printf("2) Рандомными числами от -10 до 10.\n");
    printf("Выбор: \n");

    size_t action;
    if ((*operationResult = input_action(&action, 2)) == END_OF_FILE_FOUND) return;

    switch(action) {
        case 1:
            *operationResult = fillMatrix(matrix);
            break;
        case 2:
            fillMatrixByRandom(matrix);
            break;
    }
}

void getRowsAndColsMenu(size_t* rows, size_t* cols, Err* operationResult)
{
    printf("Введите количество строк: ");
    if (input_size_t(rows) == END_OF_FILE_FOUND) {
        *operationResult = END_OF_FILE_FOUND;
        return;
    }
    printf("Введите количество столбцов: ");
    if (input_size_t(cols) == END_OF_FILE_FOUND) {
        *operationResult = END_OF_FILE_FOUND;
        return;
    }
    *operationResult = MATRIX_OPERATION_OK;
}