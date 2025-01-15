#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "head.h"
#include <string.h>

int products_generate(struct Product **products, size_t *pos) {
    printf("Введите количество генерируемых структур: ");
    input_int(pos);
    printf("%zu \n", *pos);
    *products = malloc(*pos * sizeof(struct Product));
    if (*products == NULL) {
        return 1;
    }

    srand(time(NULL));

    for (size_t i = 0; i < *pos; i++) {

        char idG[9];					// ID
        for (int j = 0; j < 8; j++) {
            idG[j] = generate();
        }
        idG[8] = '\0';
        strcpy((*products)[i].id, idG);

        int len = (int)generate() % 20 + 1;		// NAME
        (*products)[i].name = malloc(len + 1);
        if ((*products)[i].name == NULL) {
            return 1;
        }
        for (int j = 0; j < len; j++) {
            (*products)[i].name[j] = generate();
        }
        (*products)[i].name[len] = '\0';

        (*products)[i].count = generateCount();		// COUNT
    }
    return 0;
}
char generate() {
    int r = rand() % 62;
    if (r < 26) {
        return 'A' + r;
    } else if (r < 52) {
        return 'a' + (r - 26);
    } else {
        return '0' + (r - 52);
    }
}

int generateCount() {
    return (rand() % 2147483647) + 1;
}
