#include <stdlib.h>
#include <stdio.h>
#include "err.h"

#include "list.h"

/* add a character to a list */
void listAdd(listNode** head, char newch) {
	listNode* new;
	listNode* tmp;
	listNode* tail;

	/* allocate new node */
	new = malloc(1 * sizeof(listNode));
	if(new == NULL) {
		error(ERR_MALLOC);
	}

	/* initialize new node */
	new->ch = newch;
	new->next = NULL;

	/* link new node */
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

/* free a list */
void listFree(listNode** head) {
	listNode* tmp;

	tmp = *head;

	/* loop through the list freeing each node */
	while(*head != NULL) {
		tmp = (listNode*) (*head)->next;
		free(*head);
		*head = tmp;
	}

	return;
}

/* print a list -- for debugging */
void listPrint(listNode** head) {
	listNode* tmp;

	tmp = *head;

	/* header */
	fprintf(stdout, "LIST: ");

	/* contents, character by character */
	while(tmp != NULL) {
		fprintf(stdout, "-> '%c' ", tmp->ch);
		tmp = (listNode*) tmp->next;
	}
	fprintf(stdout, "\n");

	return;
}

/* convert a list of chars to a string */
char* listToString(listNode* head) {
	char* str;
	int i;
	int size;
	listNode* tmp;

	size = 0;
	tmp = head;

	/* count the nodes */
	while(tmp != NULL) {
		size++;
		tmp = (listNode*) tmp->next;
	}

	/* allocate new string of appropriate length */
	str = malloc(size * sizeof(char) + 1);
	if(str == NULL) {
		error(ERR_MALLOC);
	}

	/* add each character from the list to the string */
	tmp = head;
	for(i = 0; i < size; i++) {
		str[i] = tmp->ch;
		tmp = (listNode*) tmp->next;
	}
	str[i] = '\0'; 

	return str;
}
