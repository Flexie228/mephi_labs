#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <readline/readline.h>
#include "head.h"

int product_new(struct Product **products, size_t *pos)
{
    size_t cnt_strct;
    int flag;
    do {
	printf("Введите количество экземпляров структур: ");
	flag = input_int(&cnt_strct);
	if (flag == -1) return -1;
    } while(flag != 0);
    struct Product *b = realloc(*products, ((*pos) + cnt_strct) * sizeof(struct Product));
    if (b == NULL) {
        return 1;
    }
    *products = b;

    for (size_t i = *pos; i < *pos + cnt_strct; i++) {
        char *id_input = NULL;					// ID (8 символов)
        do {
	    id_input = readline(PROMPT_ID);
	    if (id_input == NULL) {
	    	return -1;
	    }
	    if (strlen(id_input) != 8) {
	        printf("Длина ID должна быть 8 символов!");
	    }
        } while (strlen(id_input) != 8);
	strcpy((*products)[i].id, id_input);
	free(id_input);

	char *name_input = readline(PROMPT_NAME);		// NAME
    	if (name_input == NULL) {
            return -1;
        }
    	(*products)[i].name = malloc((strlen(name_input) + 1) * sizeof(char));
    	if ((*products)[i].name == NULL) {
            free(name_input);
            return 1;
	}
	strcpy((*products)[i].name, name_input);
	free(name_input);

    	printf(PROMPT_COUNT);					// COUNT
    	size_t count_input;
    	flag = input_int(&count_input);
    	if (flag == -1) {
	    return -1;
        }
	(*products)[i].count = count_input;
    }
    (*pos) += cnt_strct;
    return 0;
}

int read_file(struct Product **products, size_t *pos,const char *name)
{
    FILE *file = fopen(name, "r");
    if (file == NULL) return 2;

    if (fscanf(file, "Products: %zu\n", pos) != 1) {
	fclose(file);
	return 3;
    }

    *products = malloc(sizeof(struct Product) * (*pos));
	if (*products == NULL) {
	fclose(file);
        return 1;
    }

    int errflag = 0;
    size_t i;
    for (i = 0; i < (*pos); i++) {

	if (fscanf(file, "ID: %8s\n", (*products)[i].id) != 1) {
	    errflag = 3;
            break;
        }

	char name_buffer[256];
        if (fscanf(file, "Name: %255s\n", name_buffer) != 1) {
	    errflag = 3;
            break;
        }
        (*products)[i].name = malloc((strlen(name_buffer) + 1) * sizeof(char));
        if ((*products)[i].name == NULL) {
	    errflag = 1;
            break;
        }
        strcpy((*products)[i].name, name_buffer);

	if (fscanf(file, "Count: %zu\n", &(*products)[i].count) != 1) {
	    errflag = 3;
            break;
        }
    }
    if (errflag != 0) {
	for (size_t j = 0; j < i ; j++) free((*products)[j].name);
	free(*products);
	fclose(file);
	return (errflag == 1) ? 1 : 3;
    }
    printf("Считано!\n");
    fclose(file);
    return 0;
}

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
