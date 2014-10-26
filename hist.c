#include <stdlib.h>
#include <stdio.h>
#include "types.h"

#include "hist.h"

/*
struct _histNode {
	int index;
	command* cmd;
	struct _histNode* prev;
	struct _histNode* next;
};
typedef struct _histNode histNode;
*/

int histCounter = 0;
histNode* histCurrent = NULL;

int histInit(void) {
	int err;
	int i;
	histNode* first;
	histNode* last;

	err = 0;
	while(!err && i < 20) {
		last = (histNode*) malloc(sizeof(histNode));
		if(last == NULL) {
			err = 1;
			break;
		}

		last->index = -1;
		last->cmd = NULL;
		last->prev = histCurrent;
		last->next = NULL;

		if(histCurrent == NULL) {
			first = last;
		} else {
			histCurrent->next = last;
		}
		histCurrent = last;

		i++;
	}

	first->prev = last;
	last->next = first;

	return err;
}

int histPrint(void) {
	int i;
	histNode* n;

	n = histCurrent;
	while(n->index != -1 && n->index > (n->prev)->index) {
		i=0;
		while((*(n->cmd))[i] != NULL) {
			fprintf(stdout, "%s ", (*(n->cmd))[i]);
			i++;
		}
		fprintf(stdout, "\n");

		n = n->prev;
	}

	return 0;
}

int histAdd(command* cmd, FILE* histfile) {
	int i = 0;

	histCurrent = histCurrent->next;
	histCurrent->cmd = cmd;
	histCurrent->index = ++histCounter;

	while((*cmd)[i] != NULL) {
		fprintf(histfile, "%s ", (char*) (*cmd)[i]);
		i++;
	}
	fprintf(histfile, "\n");

	return 0;
}
