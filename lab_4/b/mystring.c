#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "head.h"

char *myreadline()
{
    char temp[BUFSIZ] = {0};
    char *res = NULL;
    int len = 0;
    int n = 0;

    printf(PROMPT);

    do {
	n = scanf("%[^\n]", temp);
	if (n < 0) {
	    if (!res) {
		printf("Обнаружен конец файла\n");
		return NULL;
	    }
	}
	else if (n > 0) {
	    int chunk_len = mystrlen(temp);
	    int str_len = len + chunk_len;
	    res = realloc(res, str_len + 1);
	    mymemcpy(res + len, temp, chunk_len);
	    len = str_len;
	}
	else {
	    scanf("%*c");
	}
    } while (n > 0);

    if (len > 0) {
	res[len] = '\0';
    }
    else {
	res = calloc(1, sizeof(char));
    }
    return res;
}

size_t mystrlen(const char *str)
{
    size_t len = 0;
    while (str[len] != '\0') {
	len++;
    }
    return len;
}

void *mymemcpy(char *paste, const char *copy, size_t n)
{
    if (paste == NULL || copy == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < n; i++) {
	paste[i] = copy[i];
    }
    return paste;
}

char *mystrncpy(char *paste, const char *copy, size_t n)
{
    if (paste == NULL || copy == NULL) {
        return NULL;
    }
    size_t i;
    for (i = 0; i < n && copy[i] != '\0'; i++) {
        paste[i] = copy[i];
    }
    for (i; i < n; i++) {
	paste[i] = '\0';
    }
    return paste;
}

size_t mystrspn(const char *cs, const char *ct)
{
    size_t len = 0, i, j;
    for (i = 0; cs[i] != '\0'; i++) {
	int flag = 0;
        for (j = 0; ct[j] != '\0'; j++) {
            if (cs[i] == ct[j]) {
                len++;
		flag = 1;
                break;
            }
        }
        if (flag == 0) {
            break;
        }
    }
    return len;
}

size_t mystrcspn(const char *cs, const char *ct)
{
    size_t len = 0, i, j;
    for (i = 0; cs[i] != '\0'; i++) {
	for (j = 0; ct[j] != '\0'; j++) {
	    if (cs[i] == ct[j]) {
		return len;
	    }
	}
	len++;
    }
    return len;
}

char *mystrtok(char *s, const char *ct) //костыль костылевский
{
    static char *ptr = NULL;
    if (s != NULL) {
        ptr = s;
    }
    if (ptr == NULL) {
        return NULL;
    }
    ptr += mystrspn(ptr, ct);
    if (*ptr == '\0') {
        return NULL;
    }
    char *start_ptr = ptr;
    ptr += mystrcspn(ptr, ct);
    if (*ptr == '\0') {
        ptr = NULL;
    } else {
        *ptr = '\0';
        ptr++;
    }
    return start_ptr;
}

char *mystrdup(const char *cs)
{
    size_t len = mystrlen(cs) + 1;
    char *t = malloc(len);
    if (t == NULL) {
	return NULL;
    }
    for (size_t i = 0; i < len - 1; i++) {
	t[i] = cs[i];
    }
    t[len - 1] = '\0';
    return t;
}
