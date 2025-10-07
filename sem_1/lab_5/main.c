#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "head.h"

int main(int argc, char *argv[])
{
    struct Product *products = NULL;
    char *input_name = NULL;
    char *output_name = NULL;
    size_t pos = 0;
    int status = 0;
    int bin_input = 0;
    int bin_output = 0;
    int sortflag = 0;

    int opt;
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
                sortflag = 1;
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
	status = read_file(&products, &pos, input_name);
    } else if (input_name != NULL  && bin_input == 1) {
	status = read_file_bin(&products, &pos, input_name);
    } else {
	status = product_new(&products, &pos);
    }

    if (sortflag == 1 && status == 0) {
	sort(&products, &pos);
    }

    if (bin_output == 0 && status == 0) {
	product_print(products, pos, output_name);
    } else if (status == 0) {
	product_print_bin(products, pos, output_name);
    }

    switch(status) {
	case 0:
            break;
	case -1:
	    printf(PROMPT_EOF);
	    break;
	case 1:
	    printf(PROMPT_ERRALLOC);
            break;
	case 2:
	    printf("Произошла ошибка открытия файла\n");
            break;
	case 3:
	    printf("Файл поврежден или не соответсвует формату\n");
            break;
    }
    return 0;
}
