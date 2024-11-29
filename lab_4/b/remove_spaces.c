#include <stdio.h>
#include <stdlib.h>
#include "head.h"

char *remove_spaces(const char *str, int *count)
{
    printf("Ваша строчка: \"%s\"\n", str);
    int len = 0;
    int word_count = 0;
    int str_len = mystrlen(str);
    char *res = calloc(str_len + 1, sizeof(char));
    if (res == NULL) {
        printf("Ошибка выделения памяти\n");
        return NULL;
    }
    char *s = mystrdup(str);
    if (s == NULL) {
        printf("Ошибка выделения памяти\n");
        return NULL;
    }
    char *word = mystrtok(s, SYMBOLS);
    while (word != NULL) {
        int word_len = mystrlen(word);
        mystrncpy(res + len, word, word_len);
        len += word_len;
        res[len] = ' ';
        len++;
        word = mystrtok(NULL, SYMBOLS);
        word_count++;
    }
    free(s);
    if (len > 0) {
        len--;
    }
    res[len]='\0';
    char *b = realloc(res, (len+1)*sizeof(char));
    if (b == NULL) {
        printf("Ошибка выделения памяти");
        return NULL;
    }
    res = b;
    printf("Получите адекватную строчку: \"%s\"\n", res);
    *count = word_count;
    return res;
}
