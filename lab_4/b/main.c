#include <stdio.h>
#include <stdlib.h>
#include "head.h"

int main()
{
    char *str = myreadline();
    while (str != NULL) {
        int count = 0;
        int sorted_str = 0;
        char *new_str = remove_spaces(str, &count);
        if (new_str != NULL) {
            sorted_str = sort(new_str, count);
	}
	if (sorted_str != 0 || new_str == NULL) {
	    free(str);
	    free(new_str);
            return 1;
        }
        free(str);
        free(new_str);
        str = myreadline();
    }
    return 0;
}
