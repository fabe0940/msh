#include <stdlib.h>
#include <stdio.h>
#include "err.h"

#include "list.h"

void listAdd(listNode** head, char newch) {
	listNode* new;
	listNode* tmp;
	listNode* tail;

	new = malloc(1 * sizeof(listNode));
	if(new == NULL) {
		error(ERR_MALLOC);
	}

	new->ch = newch;
	new->next = NULL;

	if(*head != NULL) {
		tail = *head;
		while(tail != NULL) {
			tmp = tail;
			tail = (struct _listNode*) tail->next;
		}

		tail = tmp;
		tail->next = (struct _listNode*) new;
	} else {
		*head = new;
	}

	return;
}

void listFree(listNode** head) {
	listNode* tmp;

	tmp = *head;

	while(*head != NULL) {
		tmp = (listNode*) (*head)->next;
		free(*head);
		*head = tmp;
	}

	return;
}

void listPrint(listNode** head) {
	listNode* tmp;

	tmp = *head;

	fprintf(stdout, "LIST: ");

	while(tmp != NULL) {
		fprintf(stdout, "-> '%c' ", tmp->ch);
		tmp = (listNode*) tmp->next;
	}
	fprintf(stdout, "\n");

	return;
}

char* listToString(listNode* head) {
	char* str;
	int i;
	int size;
	listNode* tmp;

	size = 0;
	tmp = head;

	while(tmp != NULL) {
		size++;
		tmp = (listNode*) tmp->next;
	}

	str = malloc(size * sizeof(char) + 1);
	if(str == NULL) {
		error(ERR_MALLOC);
	}

	tmp = head;
	for(i = 0; i < size; i++) {
		str[i] = tmp->ch;
		tmp = (listNode*) tmp->next;
	}
	str[i] = '\0'; 

	return str;
}
