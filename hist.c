#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "err.h"
#include "hist.h"

histNode* histCurrent = NULL;

/* initialize history structures */
int histInit(histNode** hist) {
	int i;
	histNode* first;
	histNode* last;

	/* build circular doubly-linked list of history nodes */
	for(i = 0; i < 20; i++) {
		/* create node */
		last = (histNode*) malloc(sizeof(histNode));
		if(last == NULL) {
			error(ERR_MALLOC);
		}

		/* initialize node */
		last->index = -1;
		last->cmd = NULL;
		last->prev = (*hist);
		last->next = NULL;

		/* link node */
		if((*hist) == NULL) {
			first = last;
		} else {
			(*hist)->next = last;
		}
		(*hist) = last;
	}

	/* link the ends of the circle */
	first->prev = last;
	last->next = first;

	return 0;
}

/* print the contents of history */
int histPrint(histNode** hist) {
	histNode* n;

	/* loop through history in reverse chronological order */
	n = (*hist)->next;
	do {
		/* for each nonempty node */
		if(n->index != -1) {
			/* print out command line and number */
			fprintf(stdout, "%i: %s", n->index, n->cmd);
			if((n->cmd)[strlen(n->cmd) - 1] != '\n') {
				fprintf(stdout, "\n");
			}
		}
		n = n->next;
	} while(n != (*hist)->next);

	return 0;
}

/* add a command to history */
int histAdd(histNode** hist, char* cmd, FILE* histfile) {
	static int histCounter = 0;
	char* str;

	/* copy string */
	str = malloc(strlen(cmd) * sizeof(char) + 1);
	if(str == NULL) {
		error(ERR_MALLOC);
	}

	strcpy(str, cmd);

	/* update history structure */
	(*hist) = (*hist)->next;
	(*hist)->cmd = str;
	(*hist)->index = ++histCounter;

	/* optionally update history file */
	if(histfile != NULL) {
		fprintf(histfile, "%s", cmd);
	}


	return 0;
}
