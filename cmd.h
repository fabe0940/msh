#ifndef CMD_H
#define CMD_H

#include "list.h"

struct _cmdNode {
	listNode* command;
	struct _cmdNode* next;
};
typedef struct _cmdNode cmdNode;

#endif
