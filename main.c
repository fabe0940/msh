#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "clean.h"
#include "cmd.h"
#include "eval.h"
#include "exec.h"
#include "init.h"
#include "read.h"
#include "status.h"
#include "update.h"

/* msh main loop */
int main(int argc, char** argv) {
	int i;
	status s;

	mshInit(&s);

	for(i = 1; i < argc; i++) {
		fprintf(stdout, "%s\n", argv[i]);
	}

	while(s.running) {
		mshRead(&s);
		mshEval(&s);
		mshExec(&s);
		mshUpdate(&s);
	}

	mshClean(&s);

	exit(0);
}
