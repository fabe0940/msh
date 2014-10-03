#include <stdlib.h>
#include <stdio.h>
#include "list.h"

/* add a character to a list */
void listAdd(struct node** head, char newch) {
	struct node* new;
	struct node* tmp;
	struct node* tail;

	/* allocate new node */
	new = malloc(1 * sizeof(struct node));
	if(new == NULL) return;

	/* initialize new node */
	(*new).ch = newch;
	(*new).next = NULL;

	/* add new node to list */
	if(*head != NULL) {
		/* nonempty list */

		/* find the end of the list */
		tail = *head;
		while(tail != NULL) {
			tmp = tail;
			tail = (*tail).next;
		}

		/* move tail back to the last node in the list */
		tail = tmp;

		/* link in the new node */
		(*tail).next = new;
	} else {
		/* empty list - return the new node */
		*head = new;
	}

	return;
}

/* free a list */
void listFree(struct node** head) {
	struct node* tmp;

	tmp = *head;

	/* traverse the list freeing each node */
	while(*head != NULL) {
		tmp = (**head).next;
		free(*head);
		*head = tmp;
	}

	return;
}

/* print a list */
void listPrint(struct node** head) {
	struct node* tmp;

	tmp = *head;

	/* print header */
	fprintf(stdout, "LIST: ");

	/* traverse the list printing each character */
	while(tmp != NULL) {
		fprintf(stdout, "-> '%c' ", (*tmp).ch);
		tmp = (*tmp).next;
	}

	/* add a newline for pretty output */
	fprintf(stdout, "\n");

	return;
}

/* convert a list to a string (char[]) */
char* listToString(struct node* head) {
	char* str;
	int i;
	int size;
	struct node* tmp;

	size = 0;
	tmp = head;

	/* count the nodes */
	while(tmp != NULL) {
		size++;
		tmp = (*tmp).next;
	}

	/* allocate a string long enough to fit every character and a '\0' */
	str = malloc(size * sizeof(char) + 1);
	if(str == NULL) return NULL;

	tmp = head;

	/* traverse the list adding each character to the string */
	for(i = 0; i < size; i++) {
		str[i] = (*tmp).ch;
		tmp = (*tmp).next;
	}

	/* null terminate the string */
	str[i] = '\0'; 

	return str;
}
