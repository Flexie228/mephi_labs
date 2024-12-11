#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    char *output_filename = NULL;
    int opt;
    FILE *output_file = NULL;
    char buffer[BUFFER_SIZE];

    while ((opt = getopt(argc, argv, "o:")) != -1) {
        switch (opt) {
            case 'o':
                output_filename = optarg;
                break;
            case '?':
                fprintf(stderr, "Недопустимая опция: %c\n", optopt);
                return 1;
            default:
                fprintf(stderr, "Неизвестная ошибка\n");
                return 1;
        }
    }

    if (output_filename == NULL) {
        fprintf(stderr, "Имя выходного файла не указано (-o <filename>)\n");
        return 1;
    }

    output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        fprintf(stderr, "Ошибка открытия файла: %s\n", output_filename);
        perror("fopen");
        return 1;
    }

    printf("Вводите текст (для завершения ввода введите EOF):\n");

    while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
        if (strcmp(buffer, "EOF\n") == 0) { // Проверка на EOF
            break;
        }
        fputs(buffer, output_file); // Запись в файл
    }

    fclose(output_file);
    printf("Данные успешно записаны в файл: %s\n", output_filename);
    return 0;
}
