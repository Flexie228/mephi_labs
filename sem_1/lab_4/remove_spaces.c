#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "head.h"
char *remove_spaces(const char *str, int *count)
{
    printf("Ваша строчка: \"%s\"\n", str);
    int len = 0;
    int word_count = 0;
    int str_len = strlen(str);
    char *res = calloc(str_len + 1, sizeof(char));
    if (res == NULL) {
        printf("Ошибка выделения памяти.\n");
        return NULL;
    }
    char *s = strdup(str);
    if (s == NULL) {
        printf("Ошибка выделения памяти.\n");
        return NULL;
    }
    char *word = strtok(s, SYMBOLS);
    while (word != NULL) {
        int word_len = strlen(word);
        strncpy(res + len, word, word_len);
        len += word_len;
        res[len] = ' ';
        len++;
        word = strtok(NULL, SYMBOLS);
        word_count++;
    }
    free(s);
    if (len > 0) {
        len--;
    }
    res[len]='\0';
    char *b = realloc(res, (len+1)*sizeof(char));
    if (b == NULL) {
        printf("Ошибка выделения памяти.");
        return NULL;
    }
    res = b;
    printf("Получите адекватную строчку: \"%s\"\n", res);
    *count = word_count;
    return res;
}
