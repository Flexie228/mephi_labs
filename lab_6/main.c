#include <stdio.h>
#include "head.h"

int main()
{
	int status;
	Node *head = NULL;
	Node *tail = NULL;

	status = initList(&head, &tail);
	while (status == 0) {
		printList(head);
		spaceDelete(&head);
		status = modeList(&head);
		if (status == 0) {
			printList(head);
			freeList(head);
			head = NULL;
			tail = NULL;
			getchar();
			status = initList(&head, &tail);
		}
	}
	if (status == -1) {
		printf("Обнаружен конец файла.\n");
	} else {
		printf("Произошла ошибка выделения памяти.\n");
		return 1;
	}
	return 0;
}
