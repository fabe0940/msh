#include <stdlib.h>
#include <stdio.h>
#include "list.h"

void listAdd(struct node** head, char newch) {
	struct node* new;
	struct node* tmp;
	struct node* tail;

	new = malloc(1 * sizeof(struct node));
	(*new).ch = newch;
	(*new).next = NULL;

	if(*head != NULL) {
		tail = *head;

		while(tail != NULL) {
			tmp = tail;
			tail = (*tail).next;
		}

		tail = tmp;

		(*tail).next = new;

	} else {
		*head = new;
	}

	return;
}

void listFree(struct node** head) {
	struct node* tmp;

	tmp = *head;

	while(*head != NULL) {
		tmp = (**head).next;
		free(*head);
		*head = tmp;
	}

	return;
}

void listPrint(struct node** head) {
	struct node* tmp;

	tmp = *head;

	fprintf(stdout, "LIST:\n");
	while(tmp != NULL) {
		fprintf(stdout, "%c -> ", (*tmp).ch);
		tmp = (*tmp).next;
	}
	fprintf(stdout, "\n");

	return;
}

char* listToString(struct node* head) {
	char* str;
	int i;
	int size;
	struct node* tmp;

	size = 0;
	tmp = head;

	while(tmp != NULL) {
		size++;
		tmp = (*tmp).next;
	}

	str = malloc(size * sizeof(char) + 1);

	tmp = head;

	for(i = 0; i < size; i++) {
		str[i] = (*tmp).ch;
		tmp = (*tmp).next;
	}

	str[i + 1] = '\0';

	return str;
}
