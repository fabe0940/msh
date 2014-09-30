#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv) {
	char** argVals = NULL;
	int argNum = 0;
	int i;
	int pid;
	int running;

	/* process arguments */
	fprintf(stdout, "%i arguments:", argc);
	for(i = 0; i < argc; i++) {
			fprintf(stdout, "  %i: %s\n", i, argv[i]);
	}

	/* initialize */
	running = 0;

	/* main loop */
	while(running) {
		/* get input */

		/* parse input */

		/* builtins */

		if(!strcmp(argVals[0], "exit")) {
			running = 0;
		}

		/* fork */

		pid = fork();

		if(pid == -1) {
			fprintf(stderr, "msh: unable to fork.\n");
			break;
		}

		/* child -- exec command */

		if(pid == 0) {
			execvp(argVals[0], argVals);
		}

		/* parent --  wait for child */

		if(pid > 0) {
		}
	}

	/* Clean Up */

	for(i = 0; i < argNum; i++) {
		free(argVals[i]);
	}

	free(argVals);

	exit(0);
}
