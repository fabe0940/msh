#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmd.h"
#include "err.h"
#include "eval.h"
#include "status.h"

void _addCmd(cmdNode** head, char* cmd);
int _numLen(int n);

void mshEval(status* s) {
	int i;
	int j;
	int index;
	char* ch;
	char* cmdStart;
	char* cmdEnd;
	char* end;
	char* str;
	char* start;
	char* tmp;
	histNode* hist;

	str = malloc(sizeof(s->command->command) + 1);
	if(str == NULL) {
		error(ERR_MALLOC);	
	}

	strcpy(str, s->command->command);

	free(s->command->command);
	free(s->command);
	s->command = NULL;

/*

REPLACE HISTORY THINGS

	start = str;
	for(end = start; *end != '\0'; end++) {
		* do nothing *
	}

	for(ch = start; ch <= end; ch++) {
		if(*ch == '!') {
			if(*(++ch) == '!' && s->history->cmd != NULL) {
				tmp = malloc(((strlen(str) - 2) + (strlen(s->history->cmd))) * sizeof(char) + 1);

				i = 0;
				j = 0;
				while(i < (int) sizeof(tmp)) {
					if(str[j] != '!') {
						tmp[i] = str[j];
						i++;
						j++;
					} else {
						strcpy(tmp + i, s->history->cmd);
						i += strlen(s->history->cmd);
						j += 2;
					}
				}
				tmp[i] = '\0';

				free(str);
				str = tmp;
				ch++;
			} else if(fprintf(stdout, "exec item %i\n", atoi(ch + 1)) && atoi(ch + 1) > 0 && atoi(ch + 1) > s->history->index - 20) {
				index = atoi(ch + 1);
				fprintf(stdout, "exec item %i\n", index);

				hist = s->history;
				while(hist->index != index) {
					hist = hist->prev;
				}

				tmp = malloc(((strlen(str) - _numLen(index)) + (strlen(s->history->prev->cmd))) * sizeof(char) + 1);

				i = 0;
				j = 0;
				while(i < (int) sizeof(tmp)) {
					if(str[j] != '!') {
						tmp[i] = str[j];
						i++;
						j++;
					} else {
						strcpy(tmp + i, s->history->cmd);
						i += strlen(s->history->cmd);
						j += _numLen(index);
					}
				}
				tmp[i] = '\0';

				fprintf(stdout, "replaced: %s", tmp);

				free(str);
				str = tmp;
				ch++;
			} else {
				fprintf(stdout, "msh: command not found\n");
			}
		}
	}
*/

	histAdd(&(s->history), str, s->histfile);

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

int _numLen(int n) {
	int result;
	if(n < 0) {
		result = _numLen(n * -1);
	} else if(n < 10) {
		result = 1;
	} else {
		result = 1 + _numLen(n / 10);
	}

	return result;
}
