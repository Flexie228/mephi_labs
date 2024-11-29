#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "head.h"

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













