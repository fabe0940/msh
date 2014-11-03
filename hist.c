#include <stdlib.h>
#include <stdio.h>
#include "err.h"

#include "hist.h"

histNode* histCurrent = NULL;

int histInit(histNode** hist) {
	int err;
	int i;
	histNode* first;
	histNode* last;

	err = 0;
	while(!err && i < 20) {
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

		i++;
	}

	first->prev = last;
	last->next = first;

	return err;
}

int histPrint(histNode** hist) {
	histNode* n;

	n = (*hist);
	while(n->index != -1 && n->index > (n->prev)->index) {
		fprintf(stdout, "%s", n->cmd);
		n = n->prev;
	}

	return 0;
}

int histAdd(histNode** hist, char* cmd, FILE* histfile) {
	static int histCounter = 0;

	(*hist) = (*hist)->next;
	(*hist)->cmd = cmd;
	(*hist)->index = ++histCounter;

	if(histfile == NULL) {
		error(ERR_NULL);
	}

	fprintf(histfile, "%s", cmd);

	return 0;
}
