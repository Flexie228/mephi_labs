#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <readline/readline.h>
#include "head.h"

int product_print(const struct Product *products, size_t pos, const char *name)
{
    FILE *file = stdout;
    if (name != NULL) {
	file = fopen(name, "w");
	if (file == NULL) return 2;
    }
    fprintf(file, PROMPT_FILE);
    fprintf(file, "%d\n", pos);
    for (size_t i = 0; i < pos; i++) {
	fprintf(file, "\nID: %s\n", products[i].id);
	fprintf(file, "Name: %s\n", products[i].name);
        fprintf(file, "Count: %d\n", products[i].count);
    }
    if (file != stdout) fclose(file);
}

int input_int(size_t *num)
{
    int s = 0;
    float input;
    do {
        s = scanf("%f", &input);
        scanf("%[^\n]");
        if (s == EOF) {
            return -1;
        }
        else if (input == 0.0) {
            printf("Ошибка ввода. Введите натурально число.\n");
	    s = 0;
        }
	else if (input != (float)((size_t)input)) {
	    printf("Ошибка ввода. Введите натурально число.\n");
	    s = 0;
	}
    } while (s == 0);
    *num = input;
    return 0;
}
