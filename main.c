#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <string.h>

#define SYMBOLS " \t"
#define PROMPT "Вводите строчку: "

char *remove_spaces(const char *str, int *count);
int sort(const char *new_str, int count);

int main()
{
    char *str = readline(PROMPT);
    while (str != NULL) {
	int count = 0;
	int sorted_str = 0;
	char *new_str = remove_spaces(str, &count);
	if (new_str != NULL) {
	    sorted_str = sort(new_str, count);
	}
	if (sorted_str != 0 || new_str == NULL) {
            return 1;
        }
	free(str);
	free(new_str);
	str = readline(PROMPT);
    }
    return 0;
}

char *remove_spaces(const char *str, int *count)
{
    printf("Ваша строчка: \"%s\"\n", str);
    int len = 0;
    int word_count = 0;
    int str_len = strlen(str);
    char *res = calloc(str_len + 1, sizeof(char));
    if (res == NULL) {
	printf("Ошибка выделения памяти\n");
	return NULL;
    }
    char *s = strdup(str);
    if (s == NULL) {
	printf("Ошибка выделения памяти\n");
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
	printf("Ошибка выделения памяти");
	return NULL;
    }
    res = b;
    printf("Получите адекватную строчку: \"%s\"\n", res);
    *count = word_count;
    return res;
}

int sort(const char *new_str, int count)
{
    char *s = strdup(new_str);
    int *len = malloc(sizeof(int) * count);
    char **words = malloc(sizeof(char *) * count);
    if (s == NULL || len == NULL || *words == NULL) {
        printf("Ошибка выделения памяти\n");
        return 1;
    }
    char *word = strtok(s, SYMBOLS);
    for (int i = 0; i < count && word != NULL; i++) {
	words[i] = strdup(word);
	if (words[i] == NULL) {
	    printf("Ошибка выделения памяти\n");
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













