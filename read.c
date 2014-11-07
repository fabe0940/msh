#include <stdlib.h>
#include <stdio.h>
#include "cmd.h"
#include "err.h"
#include "hist.h"
#include "list.h"
#include "status.h"

#include "read.h"

void _readPrompt(void);
listNode* _readGetChars(void);

/* get a line of input */
void mshRead(status* s) {
	cmdNode* c;
	listNode* head;

	/* prompt for input */
	_readPrompt();

	/* get a list of characters from stdin */
	head = _readGetChars();
	if(head == NULL) {
		error(ERR_NULL);
	}

	/* create a new command node */
	c = malloc(sizeof(cmdNode));
	if(c == NULL) {
		error(ERR_MALLOC);	
	}

	/* link in node and input */
	c->command = head;
	c->next = NULL;
	s->command = c;
	
	return;
}

/* print the prompt */
void _readPrompt(void) {
	fprintf(stdout, "?: ");

	return;
}

/* get a line of characters from stdin */
listNode* _readGetChars(void) {
	char buff;
	listNode* l;

	l = NULL;

	/* add character to the list until newline is reached */
	do {
		buff = (char) getchar();
		listAdd(&l, buff);
	} while(buff != '\n');

	return l;
}
