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
    int bin_input = 0;
    int bin_output = 0;
    int flagsort = 0;
    struct Product *products = NULL;
    size_t pos = 0;
    char *input_name = NULL;
    char *output_name = NULL;
    while ((opt = getopt(argc, argv, "i:o:abs")) != -1) {
	switch (opt) {
	    case 'i':
		input_name = optarg;
		break;
	    case 'o':
		output_name = optarg;
		break;
	    case 'a':
                bin_input = 1;
                break;
	    case 'b':
		bin_output = 1;
		break;
	    case 's':
                flagsort = 1;
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
    if (input_name != NULL  && bin_input == 0) {
	read_file(&products, &pos, input_name);
    } else if (input_name != NULL  && bin_input == 1) {
	read_file_bin(&products, &pos, input_name);
    } else {
	product_new(&products, &pos);
    }

    if (flagsort = 1) {
	sort(&products, &pos);
    }

    if (bin_output == 0) {
	product_print(products, pos, output_name);
    } else {
	product_print_bin(products, pos, output_name);
    }
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

void product_print(const struct Product *products, size_t pos, const char *name)
{
    FILE *file = stdout;
    if (name != NULL) {
	file = fopen(name, "w");
	if (file == NULL) return;
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
void product_print_bin(const struct Product *products, size_t pos, const char *name)
{
    FILE *file = fopen(name, "wb");
    if (file == NULL) return;
    fwrite(&pos, sizeof(pos), 1, file);
    for (size_t i = 0; i < pos; i++) {
	fwrite(products[i].id, sizeof(products[i].id), 1, file);		// ID
	size_t name_len = strlen(products[i].name);				// NAME
	fwrite(&name_len, sizeof(name_len), 1, file);
	fwrite(products[i].name, sizeof(char), name_len, file);
	fwrite(&products[i].count, sizeof(products[i].count), 1, file);	// COUNT
    }
    fclose(file);
    printf("Записано в бинарный файл!\n");
    return;
}

int read_file_bin(struct Product **products, size_t *pos, const char *name)
{
    FILE *file = fopen(name, "rb");
    if (file == NULL) return 1;
    fread(pos, sizeof(*pos), 1, file);
    *products = malloc(sizeof(struct Product) * (*pos));
    if (*products == NULL) {
	fclose(file);
	return 1;
    }
    for (size_t i = 0; i < *pos; i++) {
	fread((*products)[i].id, sizeof((*products)[i].id), 1, file);
	size_t name_len;
	fread(&name_len, sizeof(name_len), 1, file);
	(*products)[i].name = malloc((name_len + 1) * sizeof(char));
	fread((*products)[i].name, sizeof(char), name_len, file);
	(*products)[i].name[name_len] = '\0';
	fread(&(*products)[i].count, sizeof((*products)[i].count), 1, file);
    }
    fclose(file);
    printf("Считано из бинарного файла!\n");
    return 0;
}

int sort(struct Product **products, size_t *pos)
{
    size_t type, dir, pole, a;
    do {
	a = 0;
	printf("***Выберите тип сортировки***\n");
	printf("3 - qsort               \n");
	size_t s;
	int flag = input_int(&s);
    	if (flag == -1) {
            return -1;
	}
	type = s;
	a = 1;
    } while (a == 0);
    do {
	a = 0;
        printf("***Выберите направление***\n");
        printf("1 - по возрастанию\n");
	printf("2 - по убыванию\n");
        size_t s;
        int flag = input_int(&s);
        if (flag == -1) {
            return -1;
        }
	dir = s;
	a = 1;
    } while (a == 0);
    do {
	a = 0;
        printf("***Выберите поле***\n");
        printf("1 - ID\n");
        printf("2 - Name\n");
	printf("3 - Count\n");
        size_t s;
        int flag = input_int(&s);
        if (flag == -1) {
            return -1;
        }
        pole = s;
	a = 1;
    } while (a == 0);
    if (pole == 1) {
	
    } else if (pole == 2) {
	
    } else {
	qsort(*products, *pos, sizeof(struct Product), compare_by_count);
    }
    return 0;
}
int compare_by_count(const void *a, const void *b) {
    const struct Product *prod_a = (const struct Product *)a;
    const struct Product *prod_b = (const struct Product *)b;
    return (prod_a->count - prod_b->count); // Сравниваем по полю count
}






