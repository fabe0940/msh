#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmd.h"
#include "err.h"
#include "eval.h"
#include "list.h"
#include "status.h"

int _numDigits(int n);

/* evaluate an input string */
void mshEval(status* s) {
	int i;
	int curr;
	char* str;
	histNode* hist;
	listNode* head;

	/* clean temporary structures */
	head = s->command->command;
	free(s->command);
	s->command = NULL;

	/* substitute the previous command for each instance of "!!" */
	evalReplace(&head, "!!", s->history->cmd);

	/* substitute the appropriate command for each instance of "!N",
		where N is a command number within the last 20 commands run. */

	/* loop through the last 20 lines of history, ignoring empty or out-
		of-bounds entries */
	hist = s->history;
	curr = hist->index;
	for(i = curr; i > (curr - 20 > 0 ? curr - 20 : 0); i--) {
		do {
			hist = hist->prev;
		} while(hist->index != i && hist != s->history);

		/* if history element i exists */
		if(hist->index == i) {
			/* substitute the command in for "!i" */
			str = malloc((_numDigits(i) + 2) * sizeof(char));
			sprintf(str, "!%i", i);
			evalReplace(&head, str, hist->cmd);
		}
	}

	/* Add the command to the history */
	histAdd(&(s->history), listToString(head), s->histfile);

	/* expand the $PATH variable */
	evalReplace(&head, "$PATH", getenv("PATH"));

	/* split the command into multiple commands seperated by ";" */
	evalSplit(&(s->command), head, ';');

	return;
}

/* checks if str is present at the head of l */
int evalCompare(listNode* l, char* str) {
	int i;
	int result;
	listNode* n;

	/* loop until the end of l or str */
	n = l;
	i = 0;
	result = 1;
	while(result && n != NULL && i < (int) strlen(str)) {
		/* check for equivalence */
		result = result && (n->ch == str[i]);

		n = n->next;
		i++;
	}

	return result;
}

/* split a command in l into a list of commands in head using seperator div */
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

	/* loop over the entire input line */
	while(curr != NULL) {
		/* at each instance of the div character or at the end of the line */
		if(curr->ch == div || curr->next == NULL) {
			/* copy the portion scanned so far into a cmdNode */

			cmd = malloc(sizeof(cmdNode));
			if(cmd == NULL) {
				error(ERR_MALLOC);
			}

			cmd->command = NULL;
			cmd->next = NULL;

			for(tmp = first; tmp != curr; tmp = tmp->next) {
				listAdd(&(cmd->command), tmp->ch);
			}

			/* don't skip the final character if it's not a seperator */
			if(curr->next == NULL) {
				listAdd(&(cmd->command), tmp->ch);
			}

			/* insert the new node into the list pointed to by head */

			cmdTail = *head;
			if(cmdTail != NULL) {
				while(cmdTail->next != NULL) {
					cmdTail = cmdTail->next;
				}
				cmdTail->next = cmd;
			} else {
				*head = cmd;
			}

			/* skip the div character */
			first = curr->next;
		}

		curr = curr->next;
	}

	return;
}

/* replace each instance of find in l with repl */
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

	/* loop over the entirety of l */
	while(curr != NULL) {
		/* if a match with find is found */
		if(evalCompare(curr, find)) {
			/* insert repl into l where find was */

			/* find the end of find */
			next = curr;
			for(i = strlen(find); i > 0; i--) {
				next = next->next;
			}

			/* break the links to find */
			if(prev != NULL) {
				prev->next = NULL;
			}

			/* append repl to the preceding list */
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

			/* link in the list after find */
			tail = prev;
			while(tail->next != NULL) {
				tail = tail->next;
			}

			/* move to the end of the replacement */
			tail->next = next;
			curr = tail;
		}

		prev = curr;
		curr = curr->next;
	}

	return;
}

/* counts the number of digits in a base10 integer */
int _numDigits(int n) {
	int result;

	/* always run on positive numbers */
	if(n < 0) {
		result = _numDigits(n * -1);
	/* base case - 1 digit */
	} else if(n < 10) {
		result = 1;
	/* recursive case - more that 1 digit */
	} else {
		result = 1 + _numDigits(n / 10);
	}

	return result;
}
