#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "cmd.h"
#include "err.h"
#include "exec.h"
#include "hist.h"
#include "list.h"
#include "makearg.h"
#include "status.h"

void _exec(char** argv);

void mshExec(status* s) {
	int argc;
	int i;
	char* str;
	char** argv;
	cmdNode* cmd;
	histNode* hist;

	cmd = s->command;
	hist = s->history;

	while(cmd != NULL) {
		str = listToString(cmd->command);
		argc = makearg(str, &argv);
		free(str);

		if(argc == -1) {
			error("makearg: invalid input");
		} else if(argc == 0) {
			break;
		} else {
			if(!strcmp(argv[0], "exit")) {
				s->running = 0;
			} else if(!strcmp(argv[0], "history")) {
				histPrint(&(s->history));
			} else if(!strcmp(argv[0], "cd")) {
				chdir(argv[1]);
			} else {
				_exec(argv);
			}

			for(i = 0; argv[i] != NULL; i++) {
				free(argv[i]);
			}
			free(argv);
		}

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
