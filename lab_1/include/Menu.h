#ifndef MENU_H
#define MENU_H

#include "Matrix.h"
#include "Err.h"

void enterProgramm();

void actionMenu(Matrix** matrixArray, size_t* matrixCount, Err* operationResult);
void createMenu(Matrix** matrixArray, size_t* matrixCount, Err* operationResult);
void changeMenu(Matrix** matrixArray, size_t matrixCount, Err* operationResult);
void deleteMenu(Matrix** matrixArray, size_t* matrixCount, Err* operationResult);

void getRowsAndColsMenu(size_t* rows, size_t* cols, Err* operationResult);
void fillMenu(Matrix* matrix, Err* operationResult);

#endif