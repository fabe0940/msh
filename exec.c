#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "cmd.h"
#include "err.h"
#include "exec.h"
#include "makearg.h"
#include "status.h"

void _exec(char** argv);

void mshExec(status* s) {
	int i;
	int argc;
	char** argv;
	cmdNode* cmd;

	cmd = s->command;

	while(cmd != NULL) {
		argc = makearg(cmd->command, &argv);
		if(argc == -1) {
			error("makearg: invalid input");
		}

		_exec(argv);

		for(i = 0; i < argc; i++) {
			free(argv[i]);
		}
		free(argv);

		cmd = cmd->next;
	}

	return;
}

void _exec(char** argv) {
	int status;
	pid_t pid;

	pid = fork();

	if(pid == -1) {
		error(ERR_FORK);
	} else if(pid == 0) {
		if(execvp(argv[0], argv) == -1) {
			error(ERR_EXEC);
		}
		exit(0);
	} else if(pid > 0) {
		wait(&status);
	}
}
