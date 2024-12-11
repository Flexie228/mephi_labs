#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "abf:")) != -1) {
        switch (opt) {
            case 'a':
                printf("Опция -a установлена\n");
                break;
            case 'b':
                printf("Опция -b установлена\n");
                break;
            case 'f':
                printf("Опция -f установлена, имя файла: %s\n", optarg);
                break;
            case '?':
                fprintf(stderr, "Недопустимая опция: %c\n", optopt);
                return 1; // Возврат кода ошибки
            default:
                fprintf(stderr, "Неизвестная ошибка\n");
                return 1; // Возврат кода ошибки
        }
    }

    // Обработка оставшихся аргументов (не являющихся опциями)
    for (int index = optind; index < argc; index++) {
        printf("Оставшийся аргумент: %s\n", argv[index]);
    }

    return 0;
}
