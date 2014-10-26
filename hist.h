#ifndef HIST_H
#define HIST_H

#include <stdio.h>
#include "types.h"

struct _histNode {
	int index;
	command* cmd;
	struct _histNode* prev;
	struct _histNode* next;
};
typedef struct _histNode histNode;

int histInit(void);
int histPrint(void);
int histAdd(command* cmd, FILE* histfile);

#endif
