#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "cmd.h"
#include "err.h"
#include "eval.h"
#include "exec.h"
#include "hist.h"
#include "list.h"
#include "makearg.h"
#include "status.h"

#define STATE_ONLY 0
#define STATE_FIRST 1
#define STATE_MID 2
#define STATE_LAST 3

#define PIPE_READ 0
#define PIPE_WRITE 1

#define STDIN 0
#define STDOUT 1

#define BUFFSIZE 1048576

void _exec(cmdNode* l);

/* execute a fully evaluated list of commands */
void mshExec(status* s) {
	int argc;
	int i;
	char* str;
	char* litPath = "PATH";
	char** argv;
	cmdNode* n;
	cmdNode* cmd;
	histNode* hist;

	n = s->command;
	hist = s->history;

	/* loop through each command */
	while(n != NULL) {
		/* loop through each command in a pipe chain */
		cmd = NULL;
		evalSplit(&cmd, n->command, '|');

		/* check for builtins */
		str = listToString(cmd->command);
		argc = makearg(str, &argv);
		free(str);

		/* parsing error */
		if(argc == -1) {
			error("makearg: invalid input");
		/* no command */
		} else if(argc == 0) {
			break;
		} else {
			/* exit the shell */
			if(!strcmp(argv[0], "exit")) {
				s->running = 0;
			/* print history */
			} else if(!strcmp(argv[0], "history")) {
				histPrint(&(s->history));
			/* change directory */
			} else if(!strcmp(argv[0], "cd")) {
				chdir(argv[1]);
			/* change the PATH variable */
			/* note that export only works for PATH, i.e
				$ export PATH=newval
				This *is* case and space sensitive. */
			} else if(!strcmp(argv[0], "export")) {
				/* check for the proper number of arguments */
				if(argc != 2) {
					error(ERR_EXPORT);
				}

				/* check for well formed input */
				i = 0;
				while(argv[1][i] != '=') {
					if(argv[1][i] != litPath[i]) {
						error(ERR_EXPORT);
					}

					i++;
				}
				i++;

				/* update the path */
				setenv("PATH", argv[1] + i, 1);
			} else {
				/* not a builtin, execute it */
				_exec(cmd);
			}

			/* clean up data structures */
			for(i = 0; argv[i] != NULL; i++) {
				free(argv[i]);
			}
			free(argv);

			cmd = cmd->next;
		}

		n = n->next;
	}

	return;
}

void _exec(cmdNode* l) {
	int argc;
	int status;
	int state;
	pid_t pid;
	char inbuff[BUFFSIZE];
	char outbuff[BUFFSIZE];
	int inpipe[2];
	int outpipe[2];
	char* str;
	char** argv;
	cmdNode* n;

	memset(inbuff, '\0', BUFFSIZE);
	memset(outbuff, '\0', BUFFSIZE);

	/* loop through all commands */
	n = l;
	state = STATE_FIRST;
	while(n != NULL) {
		if(n == l && n->next == NULL) {
			state = STATE_ONLY;
		} else if(n == l) {
			state = STATE_FIRST;
		} else if (n->next == NULL) {
			state = STATE_LAST;
		} else {
			state = STATE_MID;
		}

		/* create pipes */

		if (pipe(inpipe) == -1) {
			error(ERR_PIPE);
		}

		if (pipe(outpipe) == -1) {
			error(ERR_PIPE);
		}

		/* fork a child to exec */

		if ((pid = fork()) < 0) {
			error(ERR_FORK);
		}

		if (pid == 0) {
			if(state == STATE_ONLY) {
				close(inpipe[PIPE_READ]);
				close(inpipe[PIPE_WRITE]);
				close(outpipe[PIPE_READ]);
				close(outpipe[PIPE_WRITE]);
			} else if(state == STATE_FIRST) {
				close(inpipe[PIPE_READ]);
				close(inpipe[PIPE_WRITE]);
				close(outpipe[PIPE_READ]);
				dup2(outpipe[PIPE_WRITE], STDOUT);
				close(outpipe[PIPE_WRITE]);
			} else if(state == STATE_MID) {
				dup2(inpipe[PIPE_READ], STDIN);
				close(inpipe[PIPE_READ]);
				dup2(outpipe[PIPE_WRITE], STDOUT);
				close(outpipe[PIPE_WRITE]);
			} else if(state == STATE_LAST) {
				close(inpipe[PIPE_WRITE]);
				dup2(inpipe[PIPE_READ], STDIN);
				close(inpipe[PIPE_READ]);
				close(outpipe[PIPE_READ]);
				close(outpipe[PIPE_WRITE]);
			}

			/* finalize data structures */
			str = listToString(n->command);
			argc = makearg(str, &argv);
			free(str);

			/* execute command */
			if(execvp(argv[0], argv) == -1) {
				fprintf(stderr, "msh: command not found: %s\n", argv[0]);
			}
			exit(0);
		} else {
			if(state == STATE_ONLY) {
				close(inpipe[PIPE_READ]);
				close(inpipe[PIPE_WRITE]);
				close(outpipe[PIPE_READ]);
				close(outpipe[PIPE_WRITE]);
			} else if(state == STATE_FIRST) {
				close(inpipe[PIPE_READ]);
				close(inpipe[PIPE_WRITE]);
				close(outpipe[PIPE_WRITE]);
				dup2(outpipe[PIPE_READ], STDIN);
				close(outpipe[PIPE_READ]);
			} else if(state == STATE_MID) {
				dup2(inpipe[PIPE_WRITE], STDOUT);
				close(inpipe[PIPE_WRITE]);
				dup2(outpipe[PIPE_READ], STDIN);
				close(outpipe[PIPE_READ]);
			} else if(state == STATE_LAST) {
				close(inpipe[PIPE_READ]);
				dup2(inpipe[PIPE_WRITE], STDOUT);
				close(inpipe[PIPE_WRITE]);
				close(outpipe[PIPE_READ]);
				close(outpipe[PIPE_WRITE]);
			}

			if(state == STATE_FIRST || state == STATE_MID) {
				read(STDIN, (void*) outbuff, BUFFSIZE);
			}

			if(state == STATE_MID || state == STATE_LAST) {
				write(STDOUT, (void*) inbuff, BUFFSIZE);
			}

			if(state == STATE_LAST || state == STATE_ONLY) {
				wait(&status);
			}
		}

		memcpy(inbuff, outbuff, BUFFSIZE);
		memset(outbuff, '\0', BUFFSIZE);

		n = n->next;
	}

	return;
}
