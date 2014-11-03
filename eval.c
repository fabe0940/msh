#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmd.h"
#include "err.h"
#include "eval.h"
#include "list.h"
#include "status.h"

void _split(cmdNode** head, listNode* l, char div);
int _compare(listNode* l, char* str);
void _replace(listNode** l, char* find, char* repl);

void mshEval(status* s) {
	listNode* head;

	head = s->command->command;
	free(s->command);
	s->command = NULL;

	_replace(&head, "!!", s->history->cmd);
	_replace(&head, "$PATH", getenv("PATH"));
	histAdd(&(s->history), listToString(head), s->histfile);
	_split(&(s->command), head, ';');

	return;
}

void _split(cmdNode** head, listNode* l, char div) {
	listNode* first;
	listNode* curr;
	listNode* new;
	listNode* tmp;
	cmdNode* cmd;
	cmdNode* cmdTail;

	first = l;
	curr = first;
	new = NULL;

	while(curr != NULL) {
		if(curr->ch == div || curr->next == NULL) {
			cmd = malloc(sizeof(cmdNode));
			if(cmd == NULL) {
				error(ERR_MALLOC);
			}

			cmd->command = NULL;
			cmd->next = NULL;

			for(tmp = first; tmp != curr; tmp = tmp->next) {
				listAdd(&(cmd->command), tmp->ch);
			}

			cmdTail = *head;
			if(cmdTail != NULL) {
				while(cmdTail->next != NULL) {
					cmdTail = cmdTail->next;
				}
				cmdTail->next = cmd;
			} else {
				*head = cmd;
			}

			first = curr->next;
		}

		curr = curr->next;
	}

/*
	curr = l;
	while(curr != NULL) {
		if(curr->ch == div || curr->next == NULL) {
			tmpCurr = l;
			tmpHead = NULL;
			tmpTail = NULL;
			while(tmpCurr != curr) {
				new = malloc(sizeof(listNode));
				if(new == NULL) {
					error(ERR_MALLOC);
				}

				new->ch = tmpCurr->ch;
				new->next = NULL;

				if(tmpHead == NULL) {
					tmpHead = new;
					tmpTail = tmpHead;
				} else {
					tmpTail->next = new;
					tmpTail = new;
				}

				tmpCurr = tmpCurr->next;
			}

			cmd = malloc(sizeof(cmdNode));
			if(cmd == NULL) {
				error(ERR_MALLOC);
			}

			cmd->command = tmpHead;
			cmd->next = NULL;

			if(*head == NULL) {
				*head = cmd;
			} else {
				cmdTail = *head;

				while(cmdTail->next != NULL) {
					* do nothing *
				}

				cmdTail->next = cmd;
			}

			l = curr->next;
		}

		curr = curr->next;
	}
	*/

	return;
}

int _compare(listNode* l, char* str) {
	int i;
	int result;
	listNode* n;

	n = l;
	i = 0;
	result = 1;
	while(result && n != NULL && i < (int) strlen(str)) {
		result = result && (n->ch == str[i]);
		n = n->next;
		i++;
	}

	return result;
}

void _replace(listNode** l, char* find, char* repl) {
	int i;
	listNode* curr;
	listNode* prev;
	listNode* next;
	listNode* tail;

	curr = *l;
	prev = NULL;
	next = NULL;
	tail = NULL;

	while(curr != NULL) {
		if(_compare(curr, find)) {
			next = curr;
			for(i = strlen(find); i > 0; i--) {
				next = next->next;
			}

			if(prev != NULL) {
				prev->next = NULL;
			}

			if(prev == NULL) {
				for(i = 0; i < (int) strlen(repl); i++) {
					if(repl[i] != '\n') listAdd(&prev, repl[i]);
				}

				*l = prev;
			} else {
				for(i = 0; i < (int) strlen(repl); i++) {
					if(repl[i] != '\n') listAdd(&prev, repl[i]);
				}
			}

			tail = prev;
			while(tail->next != NULL) {
				tail = tail->next;
			}

			tail->next = next;
			curr = tail;
		}

		prev = curr;
		curr = curr->next;
	}

	return;
}
