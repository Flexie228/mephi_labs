#include <stdio.h>
#include "head.h"

int main()
{
	Node *head = NULL;
	Node *tail = NULL;

	int status = initList(&head, &tail);
	printList(head);
	if (status == 0) {
		status = modeList(&head);
		printList(head);
	}
	freeList(head);

	switch(status) {
		case 1:
			printf("Произошла ошибка выделения памяти.\n");
			freeList(head);
			break;
		case -1:
			printf("Обнаружен конец файла.\n");
		default:
			break;
	}
	return 0;
}
