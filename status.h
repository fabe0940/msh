#ifndef STATUS_H
#define STATUS_H

#include "hist.h"
#include "env.h"
#include "cmd.h"

struct _status {
	int running;
	FILE* histfile;
	cmdNode* command;
	envNode* environment;
	histNode* history;
};
typedef struct _status status;

#endif
