#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#define PROMPT_ID	"Введите ID (8 символов): "
#define PROMPT_NAME	"Введите название: "
#define PROMPT_COUNT	"Введите количество: "
#define PROMPT_EOF	"Обнаружен конец файла"

struct Product {
    char id[9];
    char *name;
    size_t count;
};

int input_int(size_t *num);
int input_char(char *symbol);
int product_new(struct Product **products, size_t *pos);
void product_print(const struct Product *products, size_t pos);

int main()
{
    struct Product *products = NULL;
    size_t pos = 0;
    char symbol;
    int cycle = 0;
    do {
	printf("a - to init new pos\n");
	printf("b - to print array and quit\n");
	cycle = input_char(&symbol);
	if (cycle == -1) {
            break;
        }
	switch(symbol) {
	    case 'a':
		cycle = product_new(&products, &pos);
		break;
	    case 'b':
		product_print(products, pos);
		cycle = 1;
	}
    } while (cycle == 0);
    return 0;
}


int product_new(struct Product **products, size_t *pos)
{
    struct Product *new_product = malloc(sizeof(struct Product));
    if (new_product == NULL) {
	printf("Произошла ошибка выделения памяти!\n");
	return 1;
    }
    char *id_input = NULL;
    do {
	id_input = readline(PROMPT_ID);
	if (id_input == NULL) {
	    printf(PROMPT_EOF);
	    free(new_product);
	    return -1;
	}
	if (strlen(id_input) != 8) {
	    printf("Длина ID должна быть 8 символов!");
	}
    } while (strlen(id_input) != 8);

    char *name_input = readline(PROMPT_NAME);
    if (name_input == NULL) {
	printf(PROMPT_EOF);
        free(new_product);
        return -1;
    }
    new_product->name = malloc((strlen(name_input) + 1) * sizeof(char));
    if (new_product->name == NULL) {
        printf("Произошла ошибка выделения памяти!\n");
        free(new_product);
        free(name_input);
        return 1;
    }

    printf(PROMPT_COUNT);
    size_t count_input;
    int flag = input_int(&count_input);
    if (flag == -1) {
	free(new_product);
	return -1;
    }

    new_product->count = count_input;
    strcpy(new_product->id, id_input);
    strcpy(new_product->name, name_input);
    free(id_input);
    free(name_input);

    struct Product *b = realloc(*products, (*pos + 1) * sizeof(struct Product));
    if (b == NULL) {
        printf("Произошла ошибка выделения памяти!\n");
        free(new_product->name);
        free(new_product);
        return 1;
    }
    *products = b;
    (*products)[*pos] = *new_product;
    (*pos)++;
    free(new_product);
    return 0;
}

int input_int(size_t *num)
{
    int s = 0;
    float input;
    do {
        s = scanf("%f", &input);
        scanf("%*[^\n]");
        if (s == EOF) {
            return -1;
        }
        else if (s == 0) {
            printf("Ошибка ввода.\n");
        }
	else if (input != (float)((size_t)input)) {
	    printf("Ошибка ввода.\n");
	    s = 0;
	}
    } while (s == 0);
    return 0;
}

void product_print(const struct Product *products, size_t pos)
{
    if (products == NULL) {
        printf("ПУСТОвалов Олег\n");
        return;
    }
    printf("Список товаров: ");
    for (size_t i = 0; i < pos; i++) {
	printf("%d) ", i + 1);
	printf("ID: %s ", products[i].id);
	printf("Name %s ", products[i].name);
        printf("%d", products[i].count);
	printf("\n");
	printf("                ");
    }
}
int input_char(char *symbol)
{
    int s = 0;
    do {
        s = scanf(" %c", symbol);
        scanf("%*[^\n]");
        if (s == EOF) {
            return -1;
        }
        else if (s == 0 || (*symbol != 'a' && *symbol != 'b')) {
            s = 0;
            printf("Ошибка. Введите либо a либо b.\n");
        }
    } while (s == 0);
    return 0;
}
