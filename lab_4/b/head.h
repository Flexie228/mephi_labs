#ifndef KAF_12
#define KAF_12

#define SYMBOLS " \t"
#define PROMPT "Вводите строчку: "
char *myreadline();
char *remove_spaces(const char *str, int *count);
char *mystrncpy(char *paste, const char *copy, size_t n);
char *mystrdup(const char *cs);
char *mystrtok(char *s, const char *ct);
char *mystrpbrk(const char *cs,const char *ct);

int sort(const char *new_str, int count);
size_t mystrlen(const char *str);
size_t mystrspn(const char *cs, const char *ct);
size_t mystrcspn(const char *cs, const char *ct);

void *mymemcpy(char *paste, const char *copy, size_t n);

#endif
