#ifndef CMD_H
#define CMD_H

struct _cmdNode {
	char* command;
	struct _cmdNode* next;
};
typedef struct _cmdNode cmdNode;

#endif
