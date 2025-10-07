#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "head.h"

int sort(struct Product **products, size_t *pos, double *time)
{
    size_t type, dir, pole, a;
    clock_t start, end;
    do {
		a = 0;
		printf("***Выберите тип сортировки***\n");
		printf("1 - shaker_sort         \n");
		printf("2 - shell_sort          \n");
		printf("3 - qsort               \n");
		size_t s;
		int flag = input_int(&s);
    	if (flag == -1) return -1;
		type = s;
		a = 1;
		if (type > 3) a = 0;
    } while (a == 0);

    do {
		a = 0;
    	printf("***Выберите направление***\n");
    	printf("1 - по возрастанию\n");
	printf("2 - по убыванию\n");
        size_t s;
        int flag = input_int(&s);
        if (flag == -1) return -1;
		dir = s;
		a = 1;
		if (s > 2 || s == 0) a = 0;
    } while (a == 0);

    do {
		a = 0;
        printf("***Выберите поле***\n");
        printf("1 - ID\n");
        printf("2 - Name\n");
		printf("3 - Count\n");
        size_t s;
        int flag = input_int(&s);
		if (flag == -1) return -1;
        pole = s;
        a = 1;
        if (type > 3) a = 0;
    } while (a == 0);
    start = clock();
    switch(type) {
	int status;
	case 1:
	    switch(pole) {
		case 1:
		    if (dir == 1) {
			status = shaker_sort(*products, *pos, sizeof(struct Product), comp_char);
		    } else {
			status = shaker_sort(*products, *pos, sizeof(struct Product), comp_char_rev);
		    }
		    break;
		case 2:
		    if (dir == 1) {
                        status = shaker_sort(*products, *pos, sizeof(struct Product), comp_name);
                    } else {
                        status = shaker_sort(*products, *pos, sizeof(struct Product), comp_name_rev);
                    }
                    break;
		case 3:
                    if (dir == 1) {
                        status = shaker_sort(*products, *pos, sizeof(struct Product), comp_count);
                    } else {
                        status = shaker_sort(*products, *pos, sizeof(struct Product), comp_count_rev);
                    }
                    break;
	    }
	    if (status == 1) return 1;
	    break;
	case 2:
	    switch(pole) {
		case 1:
		    if (dir == 1) {
                        status = shell_sort(*products, *pos, sizeof(struct Product), comp_char);
                    } else {
                        status = shell_sort(*products, *pos, sizeof(struct Product), comp_char_rev);
                    }
                    break;
                case 2:
                    if (dir == 1) {
                        status = shell_sort(*products, *pos, sizeof(struct Product), comp_name);
                    } else {
                        status = shell_sort(*products, *pos, sizeof(struct Product), comp_name_rev);
                    }
                    break;
                case 3:
                    if (dir == 1) {
                        status = shell_sort(*products, *pos, sizeof(struct Product), comp_count);
                    } else {
                        status = shell_sort(*products, *pos, sizeof(struct Product), comp_count_rev);
                    }
                    break;
	    }
	    if (status == 1) return 1;
	    break;
	case 3:
            switch(pole) {
                case 1:
                    if (dir == 1) {
                        qsort(*products, *pos, sizeof(struct Product), comp_char);
                    } else {
                        qsort(*products, *pos, sizeof(struct Product), comp_char_rev);
                    }
                    break;
                case 2:
                    if (dir == 1) {
                        qsort(*products, *pos, sizeof(struct Product), comp_name);
                    } else {
                        qsort(*products, *pos, sizeof(struct Product), comp_name_rev);
                    }
                    break;
                case 3:
                    if (dir == 1) {
                        qsort(*products, *pos, sizeof(struct Product), comp_count);
                    } else {
                        qsort(*products, *pos, sizeof(struct Product), comp_count_rev);
                    }
                    break;
            }
	    break;
    }
    end = clock();
    *time = ((double)(end - start)) / CLOCKS_PER_SEC;
    return 0;
}

int shaker_sort(void *array, size_t n, size_t size, int (*cmp)(const void *, const void *))
{
    int left = 0;
    int right = n - 1;
    int lastSwap = 0;
    int swapflag;

    do {
        swapflag = 0;
	int status;
        for (int i = left; i < right; i++) {		// Слева направо
            void *a = (char *)array + i * size;		// Текущий
            void *b = (char *)array + (i + 1) * size;	// Следующий
            if (cmp(a, b) > 0) {
                status = swap(a, b, size);
                swapflag = 1;
                lastSwap = i;
		if (status == 1) return 1;		// Меняем границу
            }
        }
        right = lastSwap;

        if (swapflag == 0) {
            break;
        }

	swapflag = 0;
        for (int i = right; i > left; i--) {		// Справа налево
            void *a = (char *)array + i * size;
            void *b = (char *)array + (i - 1) * size;
            if (cmp(a, b) < 0) {
                status = swap(a, b, size);
                swapflag = 1;
                lastSwap = i;
		if (status == 1) return 1;
            }
        }
        left = lastSwap;

    } while (swapflag != 0);
    return 0;
}

int shell_sort(void *array, size_t n, size_t size, int (*cmp)(const void *, const void *))
{
    int k = 1;
    while (fib(k - 1) < (int)n) {
        int d = fib(k);

        for (int i = d; i < (int)n; i++) {
            void *temp = malloc(size);
            if (temp == NULL) {
                return 1;
            }
	    void *a = (char *)array + i * size;
            memcpy(temp, a, size);

	    int j;
	    int status;
            for (j = i; j >= d && cmp((char *)array + (j - d) * size, temp) > 0; j -= d) {
                status = swap((char *)array + j * size, (char *)array + (j - d) * size, size);
		if (status == 1) return 1;
            }
	    void *b = (char *)array + j * size;
            memcpy(b, temp, size);
            free(temp);
        }
	k++;
    }
    return 0;
}

int fib(int k)
{
    if (k == 0) return 0;
    if (k == 1) return 1;
    return fib(k - 1) + fib(k - 2);
}

int swap(void *a, void *b, size_t size)
{
    void *temp = malloc(size);
    if (temp == NULL) {
        return 1;
    }
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
    free(temp);
    return 0;
}

int comp_count(const void *a, const void *b)
{
    return ((const struct Product *)a)->count - ((const struct Product *)b)->count;
}

int comp_count_rev(const void *a, const void *b)
{
    return ((const struct Product *)b)->count - ((const struct Product *)a)->count;
}

int comp_char(const void *a, const void *b)
{
    return strcmp(((struct Product *)a)->id, ((struct Product *)b)->id);
}

int comp_char_rev(const void *a, const void *b)
{
    return strcmp(((struct Product *)b)->id, ((struct Product *)a)->id);
}

int comp_name(const void *a, const void *b)
{
    return strcmp(((const struct Product *)a)->name, ((const struct Product *)b)->name);
}

int comp_name_rev(const void *a, const void *b)
{
    return strcmp(((const struct Product *)b)->name, ((const struct Product *)a)->name);
}
