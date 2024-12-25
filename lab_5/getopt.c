#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <readline/readline.h>
#include "head.h"

int main(int argc, char *argv[])
{
    int opt;
    int aflag = 0;
    struct Product *products = NULL;
    size_t pos = 0;
    int args = 0;
    FILE *input_file = stdin;
    FILE *output_file = stdout;
    char *input_name = NULL;
    char *output_name = NULL;
    while ((opt = getopt(argc, argv, "i:o:a")) != -1) {
	switch (opt) {
	    case 'i':
		input_name = optarg;
		args++;
		break;
	    case 'o':
		output_name = optarg;
		args++;
		break;
	    case 'a':
		aflag = 1;
		break;
	    case '?':
		if (optopt == 'i') {
		    printf("Добавьте аргумент к опции i");
		} else if (optopt == 'o') {
                    printf("Добавьте аргумент к опции o");
                } else {
		    printf("Неизвестная опция %с", optopt);
		return 1;
		}
	    default:
		printf("(default)");
		return 1;
	}
    }
    if (input_name != NULL) {
	read_file(&products, &pos, input_name);
    }
    if (output_name != NULL) {
        output_file = fopen(output_name, "w");
        if (output_file == NULL) {
            perror(output_name);
            if (input_file != stdin) {
    		fclose(input_file);
	    }
	    return 1;
        }
    }
    if (input_name == NULL) {
	int tmp = product_new(&products, &pos);
    }
    product_print(products, pos, output_file);
    if (output_file != stdout) fclose(output_file);
    return 0;
}

int product_new(struct Product **products, size_t *pos)
{
    printf("Введите количество экземпляров структур.\n");
    size_t cnt_strct;
    int flag = input_int(&cnt_strct);
    if (flag == -1 || cnt_strct == 0) {
        return -1;
    }
    struct Product *b = realloc(*products, ((*pos) + cnt_strct) * sizeof(struct Product));
    if (b == NULL) {
        printf(PROMPT_ERRALLOC);
        return 1;
    }
    *products = b;

    for (size_t i = *pos; i < *pos + cnt_strct; i++) {
        char *id_input = NULL;					// ID (8 символов)
        do {
	    id_input = readline(PROMPT_ID);
	    if (id_input == NULL) {
	    	printf(PROMPT_EOF);
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
	    printf(PROMPT_EOF);
            return -1;
        }
    	(*products)[i].name = malloc((strlen(name_input) + 1) * sizeof(char));
    	if ((*products)[i].name == NULL) {
            printf(PROMPT_ERRALLOC);
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
        else if (s == 0) {
            printf("Ошибка ввода. Введите натурально число.\n");
        }
	else if (input != (float)((size_t)input)) {
	    printf("Ошибка ввода. Введите натурально число.\n");
	    s = 0;
	}
    } while (s == 0);
    *num = input;
    return 0;
}

void product_print(const struct Product *products, size_t pos, FILE *output)
{
    if (products == NULL) {
        printf("ЗДЕСЬ ПУСТО, невозможно напечатать.\n");
        return;
    }
    fprintf(output, PROMPT_FILE);
    fprintf(output, "%d\n", pos);
    for (size_t i = 0; i < pos; i++) {
	fprintf(output, "\nID: %s\n", products[i].id);
	fprintf(output, "Name: %s\n", products[i].name);
        fprintf(output, "Count: %d\n", products[i].count);
    }
}

int read_file(struct Product **products, size_t *pos,const char *name)
{
    FILE *file = fopen(name, "r");
    if (file == NULL) return 1;

    if (fscanf(file, "Products: %zu\n", pos) != 1) {
	fclose(file);
	return 1;
    }

    *products = malloc(sizeof(struct Product) * (*pos));
	if (*products == NULL) {
	fclose(file);
        return 2;
    }

    int errflag = 0;
    size_t i;
    for (i = 0; i < (*pos); i++) {

	if (fscanf(file, "ID: %8s\n", (*products)[i].id) != 1) {
	    errflag = 1;
            break;
        }

	char name_buffer[256];
        if (fscanf(file, "Name: %255s\n", name_buffer) != 1) {
	    errflag = 1;
            break;
        }
        (*products)[i].name = malloc((strlen(name_buffer) + 1) * sizeof(char));
        if ((*products)[i].name == NULL) {
	    errflag = 2;
            break;
        }
        strcpy((*products)[i].name, name_buffer);

	if (fscanf(file, "Count: %zu\n", &(*products)[i].count) != 1) {
	    errflag = 1;
            break;
        }
    }
    if (errflag != 0) {
	for (size_t j = 0; j < i ; j++) free((*products)[j].name);
	free(*products);
	fclose(file);
	return (errflag == 1) ? 1 : 2;
    }
    printf("Считано!\n");
    fclose(file);
    return 0;
}
