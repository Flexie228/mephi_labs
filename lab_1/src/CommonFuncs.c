#include <stdio.h>
#include <stdlib.h>
#include "Err.h"
#include "Matrix.h"
#include "TypeInfo.h"
#include "Integer.h"
#include "Double.h"
#include "Complex.h"

Err input_size_t(size_t *num)
{
    int s = 0;
    double input;
    do {
        s = scanf("%lf", &input);
        scanf("%*[^\n]");
        if (s == EOF) {
            return END_OF_FILE_FOUND;
        }
	    else if (input != (double)((size_t)input) || input == 0.0) {
	        printf("Ошибка ввода. Введите натурально число.\n");
	        s = 0;
	    }
    } while (s == 0);
    *num = (size_t)input;
    return MATRIX_OPERATION_OK;
}

Err input_size_t_twice(size_t *num1, size_t *num2) {
    int s = 0;
    double input1, input2;
    do {
        s = scanf("%lf %lf", &input1, &input2);
        int c;
        while ((c = getchar()) != '\n' && c != EOF); // Очистка буфера ввода

        if (s == EOF) {
            return END_OF_FILE_FOUND;
        } else if (s != 2) {
            printf("Ошибка ввода. Введите два натуральных числа, разделенных пробелом.\n");
            s = 0;
        } else if (input1 != (double)((size_t)input1) || input1 <= 0.0 ||
                   input2 != (double)((size_t)input2) || input2 <= 0.0) {
            printf("Ошибка ввода. Введите два натуральных числа, разделенных пробелом.\n");
            s = 0;
        }
    } while (s == 0);

    *num1 = (size_t)input1;
    *num2 = (size_t)input2;

    return MATRIX_OPERATION_OK;
}

Err input_int(int *num)
{
    int s = 0;
    double input;
    do {
        s = scanf("%lf", &input);
        scanf("%*[^\n]");
        if (s == EOF) return END_OF_FILE_FOUND;
        else if (input != (double)((int)input)) {
	        printf("Ошибка ввода. Введите целое число.\n");
	        s = 0;
	    }
    } while (s == 0);
    *num = (int)input;
    return MATRIX_OPERATION_OK;
}

Err input_double(double *num)
{
    int s = 0;
    double input;
    do {
        s = scanf("%lf", &input);
        scanf("%*[^\n]");
        if (s == EOF) return END_OF_FILE_FOUND;
        if (s == 0) printf ("Ошибка ввода. Введите действительное число.\n");
    } while (s == 0);
    *num = input;
    return MATRIX_OPERATION_OK;
}

Err input_action(size_t *action, size_t maxAction)
{
    int s = 0;
    double input;
    do {
        s = scanf("%lf", &input);
        scanf("%*[^\n]");
        if (s == EOF) return END_OF_FILE_FOUND;
	    else if (input != (double)((size_t)input) || input == 0.0) {
	        printf("Ошибка ввода. Действие не найдено.\n");
	        s = 0;
	    }
        else if ((size_t)input > maxAction) {
            printf("Ошибка ввода. Действие не найдено.\n");
            s = 0;
        }
    } while (s == 0);
    *action = (size_t)input;
    return MATRIX_OPERATION_OK;
}



void clearMemory(Matrix** matrixArray, size_t matrixCount)
{
    if (matrixArray != NULL) {
        for (size_t i = 0; i < matrixCount; i++) {
            if (matrixArray[i] != NULL) freeMatrix(matrixArray[i]);
        }
        free(matrixArray);
    }
    if (INT_TYPE_INFO != NULL) {
        free(INT_TYPE_INFO);
        INT_TYPE_INFO = NULL;
    }
    if (DOUBLE_TYPE_INFO != NULL) {
        free(DOUBLE_TYPE_INFO);
        DOUBLE_TYPE_INFO = NULL;
    }
    if (COMPLEX_TYPE_INFO != NULL) {
        free(COMPLEX_TYPE_INFO);
        COMPLEX_TYPE_INFO = NULL;
    }
}


const ErrorMessage errorMessages[6] = {
    {END_OF_FILE_FOUND,         "Обнаружен конец файла.\n"},
    {MEMORY_ALLOCATION_FAILED,  "Ошибка выделения памяти.\n"},
    {MATRIX_NOT_DEFINED,        "Матрица не найдена.\n"},
    {OPERATION_NOT_DEFINED,     "Операция не определена.\n"},
    {INCOMPATIBLE_MATRIX_TYPES, "Несовместимые типы матриц.\n"},
    {INDEX_OUT_OF_BOUNDS,       "Индекс за пределами допустимого диапазона.\n"}
};

void printErrorMessage(Err operationResult) {
    for (size_t i = 0; i < 6; i++) {
        if (errorMessages[i].code == operationResult) {
            printf("Ошибка: %s\n", errorMessages[i].message);
            return;
        }
    }
}