#include <stdio.h>
#include "head.h"

int main()
{
	int status;
	size_t n;
	Node *head = NULL;
	Node *tail = NULL;

	printf("Введите число, на слова не длиннее которого надо разбить существующие в строке слова: ");
	input_int(&n);
	status = initList(&head, &tail);
	while (status == 0) {
		printList(head);
		spaceDelete(&head);
		status = modeList(&head, n);
		if (status == 0) {
			printList(head);
			freeList(head);
			head = NULL;
			tail = NULL;
			status = initList(&head, &tail);
		} else {
			freeList(head);
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
