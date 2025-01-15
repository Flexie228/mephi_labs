#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "head.h"

int main(int argc, char *argv[])
{
    struct Product *products = NULL;
    char *output_name = NULL;
    size_t pos = 0;
    int status = 0;
    int bin_output = 0;
    int sortflag = 0;
    srand(time(NULL));

    int opt;
    while ((opt = getopt(argc, argv, "o:bs")) != -1) {
	switch (opt) {
	    case 'o':
		output_name = optarg;
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
    status = products_generate(&products, &pos);

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
