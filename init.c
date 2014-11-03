#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "err.h"
#include "hist.h"
#include "status.h"

#include "init.h"

void mshInit(status* s) {
	char* fname;

	s->running = 1;

	fname = malloc((1 + strlen(getenv("HOME")) + strlen("/.msh_history")) * sizeof(char));
	if(fname == NULL) {
		error(ERR_MALLOC);
	}

	sprintf(fname, "%s/%s", getenv("HOME"), ".msh_history");
	s->histfile = fopen(fname, "a");
	if(s->histfile == NULL) {
		error(ERR_FILE);
	}

	free(fname);

	s->command = NULL;
	s->environment = NULL;
	s->history = NULL;

	histInit(&(s->history));

	return;
}
