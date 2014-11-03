#ifndef HIST_H
#define HIST_H

#include <stdio.h>

struct _histNode {
	int index;
	char* cmd;
	struct _histNode* prev;
	struct _histNode* next;
};
typedef struct _histNode histNode;

int histInit(histNode** hist);
int histPrint(histNode** hist);
int histAdd(histNode** hist, char* cmd, FILE* histfile);

#endif
