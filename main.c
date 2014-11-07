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
int main(void) {
	status s;

	/* initialize */
	mshInit(&s);

	/* main loop */
	while(s.running) {
		mshRead(&s);
		mshEval(&s);
		mshExec(&s);
		mshUpdate(&s);
	}

	/* clean up */
	mshClean(&s);

	exit(0);
}
