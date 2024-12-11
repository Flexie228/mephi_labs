#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>

#define BUFSIZ		8192
#define PROMPT_ID       "Введите ID (8 символов): "
#define PROMPT_NAME     "Введите название: "
#define PROMPT_COUNT    "Введите количество: "
#define PROMPT_EOF      "Обнаружен конец файла"

struct Product {
    char id[9];
    char *name;
    size_t count;
};

int input_int(size_t *num);
int input_char(char *symbol);
int product_new(struct Product **products, size_t *pos);
void product_print(const struct Product *products, size_t pos);

int main(int argc, char *argv[])
{
    int opt;
    int args = 0;
    int iflag = 0;
    int oflag = 0;
    int aflag = 0;
    FILE *input_file = NULL;
    FILE *output_file = NULL;
    while ((opt = getopt(argc, argv, "i:o:a")) != -1) {
	switch (opt) {
	    case 'i':
		input_file = optarg;
		args++;
		iflag = 1;
		break;
	    case 'o':
		output_file = optarg;
		args++;
		oflag = 1;
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
	    default:
		print_error("(default)");
		return 1;
	}
    }
    if (argc > 1 && (!input_filename || !output_filename)) {
        print_error("Ошибка: Необходимо указать как i, так и o.");
    }
    if (input_filename) {
        input_file = fopen(input_filename, "r");
        if (input_file == NULL) {
            print_error("Ошибка открытия in файла.");
        }
    } else {
        input_file = stdin;
    }
    if (output_filename) {
        output_file = fopen(output_filename, "w");
        if (output_file == NULL) {
            print_error("Ошибка открытия out файла.");
        }
    } else {
        output_file = stdout;
    }



















