#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "head.h"

int product_print_bin(const struct Product *products, size_t pos, const char *name)
{
    FILE *file = fopen(name, "wb");
    if (file == NULL) return 2;
    fwrite(&pos, sizeof(pos), 1, file);
    for (size_t i = 0; i < pos; i++) {
	fwrite(products[i].id, sizeof(products[i].id), 1, file);		// ID
	size_t name_len = strlen(products[i].name);				// NAME
	fwrite(&name_len, sizeof(name_len), 1, file);
	fwrite(products[i].name, sizeof(char), name_len, file);
	fwrite(&products[i].count, sizeof(products[i].count), 1, file);		// COUNT
    }
    fclose(file);
    printf("Записано в бинарный файл!\n");
    return 0;
}

int read_file_bin(struct Product **products, size_t *pos, const char *name)
{
    FILE *file = fopen(name, "rb");
    if (file == NULL) return 2;
    fread(pos, sizeof(*pos), 1, file);
    *products = malloc(sizeof(struct Product) * (*pos));
    if (*products == NULL) {
	fclose(file);
	return 1;
    }
    for (size_t i = 0; i < *pos; i++) {
	fread((*products)[i].id, sizeof((*products)[i].id), 1, file);
	size_t name_len;
	fread(&name_len, sizeof(name_len), 1, file);
	(*products)[i].name = malloc((name_len + 1) * sizeof(char));
	fread((*products)[i].name, sizeof(char), name_len, file);
	(*products)[i].name[name_len] = '\0';
	fread(&(*products)[i].count, sizeof((*products)[i].count), 1, file);
    }
    fclose(file);
    printf("Считано из бинарного файла!\n");
    return 0;
}
