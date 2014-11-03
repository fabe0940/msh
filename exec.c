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

void _exec(cmdNode* l);

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

	while(n != NULL) {
		cmd = NULL;
		evalSplit(&cmd, n->command, '|');

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
				} else if(!strcmp(argv[0], "export")) {
					if(argc != 2) {
						error(ERR_EXPORT);
					}

					i = 0;
					while(argv[1][i] != '=') {
						if(argv[1][i] != litPath[i]) {
							error(ERR_EXPORT);
						}

						i++;
					}
					i++;

					setenv("PATH", argv[1] + i, 1);
				} else {
					_exec(cmd);
				}

				for(i = 0; argv[i] != NULL; i++) {
					free(argv[i]);
				}
				free(argv);
			}

			cmd = cmd->next;
		}

		n = n->next;
	}

	return;
}

#ifdef PIPING

void _exec(cmdNode* l) {
	int i;
	int argc;
	int status;
	int pfd[2];
	pid_t pid;
	char ch;
	char* str;
	char** argv;
	cmdNode* n;

	n = l;

	pid = fork();

	if(pid == -1) {
		error(ERR_FORK);
	} else if(pid == 0) {
		while(n != NULL) {
			pipe(pfd);
			pid = fork();

			if(pid == -1) {
				error(ERR_FORK);
			} else if(pid == 0) {
				close(pfd[1]);
				dup2(pfd[0], 0);
				close(pfd[0]);
				fprintf(stdout, "pid: %i\n", pid);
				while((ch = fgetc(stdin)) != EOF) {
					fputc(ch, stdout);
				}
				exit(0);
			} else if(pid > 0) {
				str = listToString(n->command);
				argc = makearg(str, &argv);
				free(str);

				close(pfd[0]);
				dup2(pfd[1], 1);
				close(pfd[1]);

				fprintf(stdout, "pid: %i\n", pid);
				for(i = 0; argv[i] != NULL; i++) {
					fprintf(stdout, "[%i] %s\n", i, argv[i]);
				}

				if(execvp(argv[0], argv) == -1) {
					error(ERR_EXEC);
				}

				wait(&status);

				exit(0);
			}

			n = n->next;
		}
		exit(0);
	} else if(pid > 0) {
		wait(&status);
	}

	return;
}

#endif

#ifdef NOPIPING

void _exec(cmdNode* l) {
	int argc;
	int status;
	pid_t pid;
	char* str;
	char** argv;
	cmdNode* n;

	n = l;
	while(n != NULL) {
		pid = fork();

		if(pid == -1) {
			error(ERR_FORK);
		} else if(pid == 0) {
			str = listToString(n->command);
			argc = makearg(str, &argv);
			free(str);

			if(execvp(argv[0], argv) == -1) {
				error(ERR_EXEC);
			}
			exit(0);
		} else if(pid > 0) {
			wait(&status);
		}

		n = n->next;
	}

	return;
}

#endif
