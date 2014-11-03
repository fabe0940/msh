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

void mshRead(status* s) {
	cmdNode* c;
	listNode* head;

	_readPrompt();

	head = _readGetChars();
	if(head == NULL) {
		error(ERR_NULL);
	}

	c = malloc(sizeof(cmdNode));
	if(c == NULL) {
		error(ERR_MALLOC);	
	}

	c->command = head;
	c->next = NULL;
	s->command = c;
	
	return;
}

void _readPrompt(void) {
	fprintf(stdout, "?: ");

	return;
}

listNode* _readGetChars(void) {
	char buff;
	listNode* l;

	l = NULL;

	do {
		buff = (char) getchar();
		listAdd(&l, buff);
	} while(buff != '\n');

	return l;
}
