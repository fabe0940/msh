#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmd.h"
#include "err.h"
#include "eval.h"
#include "list.h"
#include "status.h"

int _numDigits(int n);

void mshEval(status* s) {
	int i;
	int curr;
	char* str;
	histNode* hist;
	listNode* head;

	head = s->command->command;
	free(s->command);
	s->command = NULL;

	evalReplace(&head, "!!", s->history->cmd);

	hist = s->history;
	curr = hist->index;
	for(i = curr; i > (curr - 20 > 0 ? curr - 20 : 0); i--) {
		do {
			hist = hist->prev;
		} while(hist->index != i && hist != s->history);

		if(hist->index == i) {
			str = malloc((_numDigits(i) + 2) * sizeof(char));
			sprintf(str, "!%i", i);
			evalReplace(&head, str, hist->cmd);
		}
	}

	histAdd(&(s->history), listToString(head), s->histfile);

	evalReplace(&head, "$PATH", getenv("PATH"));

	evalSplit(&(s->command), head, ';');

	return;
}

int evalCompare(listNode* l, char* str) {
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

void evalSplit(cmdNode** head, listNode* l, char div) {
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

			if(curr->next == NULL) {
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

	return;
}

void evalReplace(listNode** l, char* find, char* repl) {
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
		if(evalCompare(curr, find)) {
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

int _numDigits(int n) {
	int result;

	if(n < 0) {
		result = _numDigits(n * -1);
	} else if(n < 10) {
		result = 1;
	} else {
		result = 1 + _numDigits(n / 10);
	}

	return result;
}
