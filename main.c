#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "list.h"
#include "makearg.h"

/* msh main loop */
int main(int argc, char** argv) {
	char buff;
	char* str;
	char** argVals;
	int argNum;
	int i;
	int running;
	int status;
	pid_t pid;
	struct node* head;

	argNum = 0;
	running = 1;
	head = NULL;
	argVals = NULL;

	/* main loop */
	while(running) {

		/* print prompt */
		fprintf(stdout, "?: ");

		/* add each character up to and including the '\n' into a linked list
		   of characters */
		do {
			buff = (char) getchar();
			listAdd(&head, buff);
		} while(buff != '\n');

		/* convert the linked list of characters into a string */
		str = listToString(head);
		
		/* convert the string into an array of strings, one string for each word */
		argNum = makearg(str, &argVals);
		if(argNum == -1) {
			fprintf(stderr, "msh: unable to parse input\n");
			break;
		}

		/* free intermediate list */
		listFree(&head);
		head = NULL;

		/* free intermediate string */
		free(str);
		str = NULL;

		/* check for the "exit" shell builtin */
		if(!strcmp(argVals[0], "exit")) {
			running = 0;
			break;
		}

		/* fork a new process */
		pid = fork();

		/* check the return value of fork() */
		if(pid == -1) {
			/* error - abort */
			fprintf(stderr, "msh: unable to fork.\n");
			break;

		} else if(pid == 0) {
			/* child - exec command */
			if(execvp(argVals[0], argVals) == -1) {
				fprintf(stderr, "msh: unable to execute %s\n", argVals[0]);
				exit(1);
			}

			exit(0);

		} else if(pid > 0) {
			/* parent - wait for child */
			wait(&status);
		}

		/* clean up data structures */
		for(i = 0; i < argNum; i++) {
			free(argVals[i]);
		}
		free(argVals);
		
	}

	exit(0);
}
