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
int product_print(const struct Product *products, size_t pos, const char *name);
int product_print_bin(const struct Product *products, size_t pos, const char *name);
int read_file(struct Product **products, size_t *pos, const char *file);
int read_file_bin(struct Product **products, size_t *pos, const char *file);
int sort(struct Product **products, size_t *pos);
int comp_count(const void *a, const void *b);
int comp_count_rev(const void *a, const void *b);
int comp_char(const void *a, const void *b);
int comp_char_rev(const void *a, const void *b);
int shaker_sort(void *array, size_t n, size_t size, int (*cmp)(const void *, const void *));
int swap(void *a, void *b, size_t size);
int shell_sort(void *array, size_t n, size_t size, int (*cmp)(const void *, const void *));
int fib(int k);
int comp_name(const void *a, const void *b);
int comp_name_rev(const void *a, const void *b);
