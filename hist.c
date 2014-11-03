#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "err.h"
#include "hist.h"

histNode* histCurrent = NULL;

int histInit(histNode** hist) {
	int i;
	histNode* first;
	histNode* last;

	for(i = 0; i < 20; i++) {
		last = (histNode*) malloc(sizeof(histNode));
		if(last == NULL) {
			error(ERR_MALLOC);
		}

		last->index = -1;
		last->cmd = NULL;
		last->prev = (*hist);
		last->next = NULL;

		if((*hist) == NULL) {
			first = last;
		} else {
			(*hist)->next = last;
		}
		(*hist) = last;
	}

	first->prev = last;
	last->next = first;

	return 0;
}

int histPrint(histNode** hist) {
	histNode* n;

	n = (*hist)->next;

	do {
		if(n->index != -1) {
			fprintf(stdout, "%i: %s", n->index, n->cmd);
			if((n->cmd)[strlen(n->cmd) - 1] != '\n') {
				fprintf(stdout, "\n");
			}
		}
		n = n->next;
	} while(n != (*hist)->next);

	return 0;
}

int histAdd(histNode** hist, char* cmd, FILE* histfile) {
	static int histCounter = 0;
	char* str;

	str = malloc(strlen(cmd) * sizeof(char) + 1);
	if(str == NULL) {
		error(ERR_MALLOC);
	}

	strcpy(str, cmd);

	(*hist) = (*hist)->next;
	(*hist)->cmd = str;
	(*hist)->index = ++histCounter;

	if(histfile != NULL) {
		fprintf(histfile, "%s", cmd);
	}


	return 0;
}
