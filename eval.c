#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmd.h"
#include "err.h"
#include "eval.h"
#include "list.h"
#include "status.h"

void _addCmd(cmdNode** head, char* cmd);
int _numLen(int n);

void mshEval(status* s) {
	int i;
	char* path;
	cmdNode* cmd;
	cmdNode* cmdTail;
	listNode* curr;
	listNode* head;
	listNode* new;
	listNode* next;
	listNode* prev;
	listNode* tmpCurr;
	listNode* tmpHead;
	listNode* tmpTail;

	head = s->command->command;
	free(s->command);
	s->command = NULL;

	curr = head;
	prev = NULL;
	while(curr != NULL) {
		if(curr->ch == '!') {
			if(curr->next != NULL && curr->next->ch == '!') {
				next = curr->next->next;

				for(i = 0; i < (int) strlen(s->history->cmd); i++) {
					if((s->history->cmd)[i] != '\n') {
						listAdd(&prev, (s->history->cmd)[i]);
					}
				}

				curr = prev;
				while(curr != NULL && curr->next != NULL) {
					curr = curr->next;
				}

				listPrint(&prev);
				fprintf(stdout, "\n\n\n");
				listPrint(&next);
				curr->next = next;
				curr = curr->next;
				head = prev;
			}
		} else if(curr->ch == '$') {
			if((curr->next != NULL && curr->next->ch == 'P') && (curr->next->next != NULL && curr->next->next->ch == 'A') && (curr->next->next->next != NULL && curr->next->next->next->ch == 'T') && (curr->next->next->next->next != NULL && curr->next->next->next->next->ch == 'H')) {
				next = curr->next->next->next->next->next;

				path = getenv("PATH");
				for(i = 0; i < (int) strlen(path); i++) {
					listAdd(&prev, path[i]);
				}

				curr = prev;
				while(curr != NULL && curr->next != NULL) {
					curr = curr->next;
				}

				listPrint(&prev);
				fprintf(stdout, "\n\n\n");
				listPrint(&next);
/*
				curr->next = next;
				listPrint(&head);
				curr = curr->next;
				head = prev;
*/
			}
		}
		prev = curr;
		curr = curr->next;
	}

	histAdd(&(s->history), listToString(head), s->histfile);

	curr = head;
	while(curr != NULL) {
		if(curr->ch == ';' || curr->next == NULL) {
			tmpCurr = head;
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

			if(s->command == NULL) {
				s->command = cmd;
			} else {
				cmdTail = s->command;

				while(cmdTail->next != NULL) {
					/* do nothing */
				}

				cmdTail->next = cmd;
			}

			head = curr->next;
		}

		curr = curr->next;
	}
/*

	str = malloc(sizeof(s->command->command) + 1);
	if(str == NULL) {
		error(ERR_MALLOC);	
	}

	strcpy(str, s->command->command);

	free(s->command->command);
	free(s->command);
	s->command = NULL;

scan:
	start = str;
	for(end = start; *end != '\0'; end++) {
		* do nothing *
	}

	for(ch = start; ch <= end; ch++) {
		if(*ch == '!') {
			if(*(++ch) == '!' && s->history->cmd != NULL) {
				tmp = malloc(((strlen(str) - 2) + (strlen(s->history->cmd))) * sizeof(char) + 1);
				if(tmp == NULL) {
					error(ERR_MALLOC);
				}

				i = 0;
				j = 0;
				while(i < (int) (sizeof(tmp) / sizeof(char))) {
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

				free(str);
				str = tmp;
				goto scan;
			}
*
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
		} else if(*ch == '$') {
			if(*(ch + 1) == 'P' && *(ch + 2) == 'A' && *(ch + 3) == 'T' && *(ch + 4) == 'H') {
				tmp = malloc(((strlen(str) - 5) + (strlen(getenv("PATH")))) * sizeof(char) + 1);
				if(tmp == NULL) {
					error(ERR_MALLOC);
				}

				i = 0;
				j = 0;
				while(i < (int) (((strlen(str) - 5) + (strlen(getenv("PATH")))) * sizeof(char)) && j < (int) strlen(str)) {
					if(str[j] != '$') {
						tmp[i] = str[j];
						i++;
						j++;
					} else {
						strcpy(tmp + i, getenv("PATH"));
						i += strlen(getenv("PATH"));
						j += 5;
					}
				}
				tmp[i] = '\0';

				free(str);
				str = tmp;
			}
		}
	}

	histAdd(&(s->history), str, s->histfile);

	start = str;
	for(end = start; *end != '\0'; end++) {
		* do nothing *
	}

	for(cmdStart = start, cmdEnd = cmdStart; cmdEnd <= end; cmdEnd++) {
		if(*cmdEnd == ';' || *cmdEnd == '\0') {
			*cmdEnd = '\0';
			_addCmd(&(s->command), cmdStart);
			cmdStart = cmdEnd + 1;
		}
	}

	free(str);
*/
}
