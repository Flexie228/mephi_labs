#ifndef KAF_12
#define KAF_12

typedef struct Node {
                char data;
                struct Node *next;
} Node;

int input_int(size_t *num);
int initList(Node **head, Node **tail);
int modeList(Node **head);
void spaceDelete(Node **head);
void printList(Node *head);
void freeList(Node *head);

#endif
