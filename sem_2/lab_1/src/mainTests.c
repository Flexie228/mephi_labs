#include <assert.h>
#include <stdio.h>
#include <TypeInfo.h>
#include <Matrix.h>
#include <Integer.h>
#include <Double.h>
#include <string.h>

void test_create_matrix() {
    Err operationResult;
    Matrix* matrix = createMatrix(GetIntTypeInfo(), 2, 3, &operationResult);
    assert(operationResult == MATRIX_OPERATION_OK);
    assert(matrix != NULL);
    assert(matrix->rows == 2);
    assert(matrix->cols == 3);
    assert(matrix->typeInfo == GetIntTypeInfo());
    assert(matrix->data != NULL);

    freeMatrix(matrix);
    printf("test_create_matrix passed\n");
}

void test_create_matrix_memory_allocation_failed() {
    Err operationResult;


    Matrix* matrix = createMatrix(GetIntTypeInfo(), 3525163520, 3525163520, &operationResult);
    assert(operationResult == MEMORY_ALLOCATION_FAILED);
    assert(matrix == NULL);

    printf("test_create_matrix_memory_allocation_failed passed\n");
}



void test_add_matrix() {
    Err operationResult;

    Matrix* matrix1 = createMatrix(GetIntTypeInfo(), 2, 2, &operationResult);
    Matrix* matrix2 = createMatrix(GetIntTypeInfo(), 2, 2, &operationResult);
    Matrix* result = createMatrix(GetIntTypeInfo(), 2, 2, &operationResult);

    int data1[] = {1, 2, 3, 4};
    int data2[] = {5, 6, 7, 8};
    memcpy(matrix1->data, data1, sizeof(data1));
    memcpy(matrix2->data, data2, sizeof(data2));

    Err status = addMatrix(matrix1, matrix2, result);
    assert(status == MATRIX_OPERATION_OK);

    int expectedData[] = {6, 8, 10, 12};
    int comparisonResult = memcmp(result->data, expectedData, sizeof(expectedData));

    assert(comparisonResult == 0);

    freeMatrix(matrix1);
    freeMatrix(matrix2);
    freeMatrix(result);
    printf("test_add_matrix passed\n");
}

void test_add_matrix_null_matrix() {
    Err operationResult;

    Matrix* matrix = createMatrix(GetIntTypeInfo(), 2, 2, &operationResult);
    Matrix* result = createMatrix(GetIntTypeInfo(), 2, 2, &operationResult);

    Err status = addMatrix(NULL, matrix, result);
    assert(status == MATRIX_NOT_DEFINED);

    freeMatrix(matrix);
    freeMatrix(result);
    printf("test_add_matrix_null passed\n");
}

void test_add_matrix_incompatible_types() {
    Err operationResult;
    Matrix* matrix1 = createMatrix(GetIntTypeInfo(), 2, 2, &operationResult);
    Matrix* matrix2 = createMatrix(GetDoubleTypeInfo(), 2, 2, &operationResult);
    Matrix* result = createMatrix(GetIntTypeInfo(), 2, 2, &operationResult);

    Err status = addMatrix(matrix1, matrix2, result);
    assert(status == INCOMPATIBLE_MATRIX_TYPES);

    freeMatrix(matrix1);
    freeMatrix(matrix2);
    freeMatrix(result);
    printf("test_add_matrix_incompatible_types passed\n");
}


void test_multiply_matrix() {
    Err operationResult;

    Matrix* matrix1 = createMatrix(GetIntTypeInfo(), 2, 2, &operationResult);
    Matrix* matrix2 = createMatrix(GetIntTypeInfo(), 2, 2, &operationResult);
    Matrix* result = createMatrix(GetIntTypeInfo(), 2, 2, &operationResult);


    int data1[] = {1, 2, 3, 4};
    int data2[] = {5, 6, 7, 8};
    memcpy(matrix1->data, data1, sizeof(data1));
    memcpy(matrix2->data, data2, sizeof(data2));

    Err status = multiplyMatrix(matrix1, matrix2, result);
    assert(status == MATRIX_OPERATION_OK);

    int expectedData[] = {19, 22, 43, 50};
    int comparisonResult = memcmp(result->data, expectedData, sizeof(expectedData));

    assert(comparisonResult == 0);

    freeMatrix(matrix1);
    freeMatrix(matrix2);
    freeMatrix(result);
    printf("test_multiply_matrix passed\n");
}

void test_multiply_matrix_null_matrix() {
    Err operationResult;
    Matrix* matrix = createMatrix(GetIntTypeInfo(), 2, 2, &operationResult);
    Matrix* result = createMatrix(GetIntTypeInfo(), 2, 2, &operationResult);

    Err status = multiplyMatrix(NULL, matrix, result);
    assert(status == MATRIX_NOT_DEFINED);

    freeMatrix(matrix);
    freeMatrix(result);
    printf("test_multiply_matrix_null_matrix passed\n");
}

void test_multiply_matrix_incompatible_types() {
    Err operationResult;
    Matrix* matrix1 = createMatrix(GetIntTypeInfo(), 2, 3, &operationResult);
    Matrix* matrix2 = createMatrix(GetIntTypeInfo(), 2, 2, &operationResult);

    Matrix* result = createMatrix(GetIntTypeInfo(), 2, 2, &operationResult);

    Err status = multiplyMatrix(matrix1, matrix2, result);
    assert(status == INCOMPATIBLE_MATRIX_TYPES);

    freeMatrix(matrix1);
    freeMatrix(matrix2);
    freeMatrix(result);
    printf("test_multiply_matrix_incompatible_types passed\n");
}
void test_multiply_matrix_null_result() {
    Err operationResult;

    Matrix* matrix1 = createMatrix(GetIntTypeInfo(), 2, 3, &operationResult); // 2x3 матрица
    Matrix* matrix2 = createMatrix(GetIntTypeInfo(), 3, 2, &operationResult);

    Err status = multiplyMatrix(matrix1, matrix2, NULL);
    assert(status == MEMORY_ALLOCATION_FAILED);

    freeMatrix(matrix1);
    freeMatrix(matrix2);
    printf("test_multiply_matrix_null_result passed\n");
}

int main() {
    test_create_matrix();
    test_create_matrix_memory_allocation_failed();

    test_add_matrix();
    test_add_matrix_null_matrix();
    test_add_matrix_incompatible_types();

    test_multiply_matrix();
    test_multiply_matrix_null_matrix();
    test_multiply_matrix_incompatible_types();
    test_multiply_matrix_null_result();
    return 0;
}