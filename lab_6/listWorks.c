#include <stdio.h>
#include <stdlib.h>
#include "head.h"

int initList(Node **head, Node **tail)
{
        printf("Введите строку: ");
        char symbol;
        while ((symbol = getchar()) != '\n') {
                if (symbol == EOF) return -1;
                Node *newNode = malloc(sizeof(Node));
                if (newNode == NULL) {
                        return 1;
                }
                newNode->data = symbol;
                newNode->next = NULL;

                if (*head == NULL) {
                        *head = newNode;
                        *tail = newNode;
                } else {
                        (*tail)->next = newNode;
                        *tail = newNode;
                }
        }
        return 0;
}

int modeList(Node **head)
{
        printf("Введите число, на слова не длиннее которого надо разбить существующие в строке слова: ");
        size_t n;
        input_int(&n);

        size_t count = 0;
        Node *currentNode = *head;
        while (currentNode != NULL) {
                if (currentNode->data != ' ') {
                        count++;
                        if (count == n) {
                                if (currentNode->next != NULL && currentNode->next->data != ' ') {
                                        Node *newNode = malloc(sizeof(Node));
                                        if (newNode == NULL) return 1;
                                        newNode->data = ' ';
                                        newNode->next = currentNode->next;
                                        currentNode->next = newNode;
                                }
                                count = 0;
                        }
                } else {
                                count = 0;
                }
                currentNode = currentNode->next;
        }
        return 0;
}

int input_int(size_t *num)
{
    int s = 0;
    float input;
    do {
        s = scanf("%f", &input);
        scanf("%[^\n]");
        if (s == EOF) {
            return -1;
        }
        else if (s == 0) {
            printf("Ошибка ввода. Введите натурально число.\n");
            s = 0;
        } else if (input != (float)((size_t)input)) {
                printf("Ошибка ввода. Введите натурально число.\n");
                s = 0;
                }
    } while (s == 0);
    *num = input;
    return 0;
}

void printList(Node *head)
{
        printf("Вы ввели: \"");
        Node *printNode = head;
        while(printNode != NULL){
                printf("%c", printNode->data);
                printNode = printNode->next;
        }
        printf("\"\n");
}

void freeList(Node *head)
{
        Node *currentNode = head;
        Node *nextNode;
        while(currentNode != NULL) {
                nextNode = currentNode->next;
                free(currentNode);
                currentNode = nextNode;
        }
}
