#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
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

		/* fork */

		pid = fork();

		if(pid == -1) {
			fprintf(stderr, "msh: unable to fork.\n");
			break;
		}

		/* child -- exec command */

		if(pid == 0) {
			/* builtins */

			/* exec */
		}

		/* parent --  wait for child */

		if(pid > 0) {
		}
	}

	exit(0);
}
