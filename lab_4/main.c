#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <time.h>
#include "head.h"

int main()
{
    char *str = readline(PROMPT);
    while (str != NULL) {
	int count = 0;
	int sorted_str = 0;
	clock_t start = clock();
	char *new_str = remove_spaces(str, &count);
	if (new_str != NULL) {
	    sorted_str = sort(new_str, count);
	}
	clock_t end = clock();
	float time = (float)(end - start) / CLOCKS_PER_SEC;
	printf("Time: %.9f\n", time);
	printf("Words: %d\n", count);
	if (sorted_str != 0 || new_str == NULL) {
            return 1;
        }
	free(str);
	free(new_str);
	str = readline(PROMPT);
    }
    return 0;
}













