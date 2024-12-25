#define BUF             1024
#define PROMPT_ID       "Введите ID (8 символов): "
#define PROMPT_NAME     "Введите название: "
#define PROMPT_COUNT    "Введите количество: "
#define PROMPT_EOF      "Обнаружен конец файла.\n"
#define PROMPT_ERRALLOC "Произошла ошибка выделения памяти!\n"
#define PROMPT_FILE     "Products: "

struct Product {
    char id[9];
    char *name;
    size_t count;
};

int input_int(size_t *num);
int product_new(struct Product **products, size_t *pos);
void product_print(const struct Product *products, size_t pos, FILE *output);
int read_file(struct Product **products, size_t *pos, const char *file);
