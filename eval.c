#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmd.h"
#include "err.h"
#include "eval.h"
#include "status.h"

void _addCmd(cmdNode** head, char* cmd);

void mshEval(status* s) {
	char* str;
	char* start;
	char* end;
	char* cmdStart;
	char* cmdEnd;
	cmdNode* cmd;

	str = malloc(sizeof(s->command->command) + 1);
	if(str == NULL) {
		error(ERR_MALLOC);	
	}

	strcpy(str, s->command->command);

	free(s->command->command);
	free(s->command);
	s->command = NULL;

	start = str;
	for(end = start; *end != '\0'; end++) {
		/* do nothing */
	}

	for(cmdStart = start, cmdEnd = cmdStart; cmdEnd <= end; cmdEnd++) {
		if(*cmdEnd == ';' || *cmdEnd == '\0') {
			*cmdEnd = '\0';
			_addCmd(&(s->command), cmdStart);
			cmdStart = cmdEnd + 1;
		}
	}

	free(str);
}

void _addCmd(cmdNode** head, char* cmd) {
	char* str;
	cmdNode* tail;
	cmdNode* new;

	new = malloc(sizeof(cmdNode));
	if(new == NULL) {
		error(ERR_MALLOC);
	}

	str = malloc(strlen(cmd)+ 1);
	if(str == NULL) {
		error(ERR_MALLOC);
	}

	strcpy(str, cmd);

	new->command = str;
	new->next = NULL;

	if(*head == NULL) {
		*head = new;
	} else {
		tail = *head;

		while(tail->next != NULL) {
			tail = tail->next;
		}

		tail->next = new;
	}

	return;
}
