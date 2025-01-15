#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "headGenerate.h"
#include <string.h>

int products_generate(struct Product **products, size_t *pos) {
    printf("Введите количество генерируемых структур: ");
    input_int(pos);

    for (size_t i = 0; i < *pos; i++) {

        char idG[9];							// ID
        for (int j = 0; j < 8; j++) {
            idG[j] = generate();
        }
        idG[8] = '\0';
		strcpy((*products)[i].id, idG);

        int len = generate() % 20 + 1;					// NAME
        char *nameG = malloc(len + 1);
        if (nameG == NULL) {
            return 1;
        }

        for (int j = 0; j < len; j++) {
            nameG[j] = generate();
        }
        nameG[len] = '\0';
		strcpy((*products)[i].name, nameG);
        free(nameG);

        int countG = generateCount(); 					// COUNT
		(*products)[i].count = countG;
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
