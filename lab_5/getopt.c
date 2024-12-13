#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <readline/readline.h>

#define BUF		2048
#define PROMPT_ID       "Введите ID (8 символов): "
#define PROMPT_NAME     "Введите название: "
#define PROMPT_COUNT    "Введите количество: "
#define PROMPT_EOF      "Обнаружен конец файла.\n"
#define PROMPT_ERRALLOC "Произошла ошибка выделения памяти!\n"
#define PROMPT_FILE	"Count of products: "

struct Product {
    char id[9];
    char *name;
    size_t count;
};

int copy_stream(FILE *input, FILE *output);
int input_int(size_t *num);
int product_new(struct Product **products, size_t *pos);
void product_print(const struct Product *products, size_t pos, FILE *output);

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
	input_file = fopen(input_name, "r");
	if (input_file == NULL) {
	    perror(input_name);
	    return 1;
	}
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
	if (tmp == 0) {
	    product_print(products, pos, output_file);
    	}
    }
    if (output_name == NULL) {
	int tmp = copy_stream(input_file, output_file);
    }
    if (input_file != stdin) fclose(input_file);
    if (output_file != stdout) fclose(output_file);
    return 0;
}

int copy_stream(FILE *input, FILE *output)
{
    char buf[BUF];
    size_t bytes_read;
    while ((bytes_read = fread(buf, 1, BUF, input)) > 0) {
        size_t bytes_written = fwrite(buf, 1, bytes_read, output);
        if (bytes_written != bytes_read) {
            printf("Ошибка записи в файл.\n");
            return 1;
        }
    }
    if (ferror(input)) {
        printf("Ошибка чтения из файла.\n");
        return 1;
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
        scanf("%*[^\n]");
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
    fprintf(output, "%d\n\n", pos);
    for (size_t i = 0; i < pos; i++) {
	fprintf(output, "ID: %s\n", products[i].id);
	fprintf(output, "Name: %s\n", products[i].name);
        fprintf(output, "%d\n\n", products[i].count);
    }
}










