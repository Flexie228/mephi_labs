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
#define PROMPT_EOF      "Обнаружен конец файла"

struct Product {
    char id[9];
    char *name;
    size_t count;
};

void copy_stream(FILE *input, FILE *output);
/*int input_int(size_t *num);
int input_char(char *symbol);
int product_new(struct Product **products, size_t *pos);
void product_print(const struct Product *products, size_t pos);
*/
int main(int argc, char *argv[])
{
    int opt;
    int args = 0;
    int iflag = 0;
    int oflag = 0;
    int aflag = 0;
    FILE *input_file = stdin;
    FILE *output_file = stdout;
    char *input_name = NULL;
    char *output_name = NULL;
    while ((opt = getopt(argc, argv, "i:o:a")) != -1) {
	switch (opt) {
	    case 'i':
		input_name = optarg;
		args++;
		iflag = 1;
		break;
	    case 'o':
		output_name = optarg;
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
		}
	    default:
		printf("(default)");
		return 1;
	}
    }
    if (input_name != NULL) {
	input_file = fopen(input_name, "r");
	if (input_file == NULL) {
	    printf("errno: %d\n", errno);
	    perror(input_name);
	    return 1;
	}
    }
    if (output_name != NULL) {
        output_file = fopen(output_name, "w");
        if (output_file == NULL) {
            printf("Ошибка выходной");
            if (input_file != stdin) {
    		fclose(input_file);
	    }
	    return 1;
        }
    }
    copy_stream(input_file, output_file);

    if (input_file != stdin) fclose(input_file);
    if (output_file != stdout) fclose(output_file);
    return 0;
}

void copy_stream(FILE *input, FILE *output)
{
    char buffer[BUF];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, BUF, input)) > 0) {
        size_t bytes_written = fwrite(buffer, 1, bytes_read, output);
        if (bytes_written != bytes_read) {
            fprintf(stderr, "Ошибка записи в файл.\n");
            exit(1);
        }
    }
    if (ferror(input)) {
        fprintf(stderr, "Ошибка чтения из файла.\n");
        exit(1);
    }
}















