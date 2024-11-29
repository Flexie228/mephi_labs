#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "head.h"

int sort(const char *new_str, int count)
{
    char *s = strdup(new_str);
    int *len = malloc(sizeof(int) * count);
    char **words = malloc(sizeof(char *) * count);
    if (s == NULL || len == NULL || words == NULL) {
        printf("Ошибка выделения памяти.\n");
        return 1;
    }
    char *word = strtok(s, SYMBOLS);
    for (int i = 0; i < count && word != NULL; i++) {
        words[i] = strdup(word);
        if (words[i] == NULL) {
            printf("Ошибка выделения памяти.\n");
            return 1;
        }
        len[i] = strlen(word);
        word = strtok(NULL, SYMBOLS);
    }
    int index;
    for (int i = 0; i < count - 1; i++) {
        index = i;
        for (int j = i + 1; j < count; j++) {
            if (len[index] > len[j]) {
                index = j;
            }
        }
        if (index != i) {
            int temp_len = len[i];
            len[i] = len[index];
            len[index] = temp_len;

            char *temp_word = words[i];
            words[i] = words[index];
            words[index] = temp_word;
        }
    }
    printf("Отсортированная по возрастанию: \"");
    for (int i = 0; i < count; i++) {
            printf("%s ", words[i]);
    }
    printf("\"\n");
    for (int i = 0; i < count; i++) {
        free(words[i]);
    }
    free(s);
    free(len);
    free(words);
    return 0;
}
