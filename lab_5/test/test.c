#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#define PROMPT "Введите название: "

struct Sample {
    size_t id;
    char *name;
    size_t count;
};

int new_sample(struct Sample **samples, size_t *pos);

int main() {
    struct Sample *samples = NULL;
    size_t pos = 0;
    
    // Allocate memory for one sample initially
    samples = malloc(sizeof(struct Sample));
    if (samples == NULL) {
        perror("Failed to allocate memory");
        return EXIT_FAILURE;
    }

    new_sample(&samples, &pos);
    
    // Print the sample details for verification
    printf("Sample ID: %zu\n", samples[0].id);
    printf("Sample Name: %s\n", samples[0].name);
    printf("Sample Count: %zu\n", samples[0].count);

    // Free allocated memory
    free(samples[0].name);
    free(samples);

    return 0;
}

int new_sample(struct Sample **samples, size_t *pos) {
    printf("Введите id: ");
    scanf("%zu", &(*samples)[*pos].id);  // Use & to store input correctly

    char *new_name = readline(PROMPT);
    if (new_name == NULL) {
        perror("Failed to read name");
        return EXIT_FAILURE;
    }

    size_t name_len = strlen(new_name);
    (*samples)[*pos].name = malloc((name_len + 1) * sizeof(char));  // Allocate memory for name
    if ((*samples)[*pos].name == NULL) {
        perror("Failed to allocate memory for name");
        free(new_name);
        return EXIT_FAILURE;
    }
    
    strcpy((*samples)[*pos].name, new_name);  // Copy the name to the allocated memory
    free(new_name);  // Free the temporary name variable

    printf("Введите количество: ");
    scanf("%zu", &(*samples)[*pos].count);  // Use & to store input correctly

    (*pos)++;
    return 0;
}
