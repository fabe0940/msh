#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "err.h"
#include "hist.h"
#include "status.h"

#include "init.h"

void mshInit(status* s) {
	size_t len;
	char* fname;
	char* buf;
	FILE* hist;

	s->command = NULL;
	s->environment = NULL;
	s->history = NULL;

	histInit(&(s->history));

	fname = malloc((1 + strlen(getenv("HOME")) + strlen("/.msh_history")) * sizeof(char));
	if(fname == NULL) {
		error(ERR_MALLOC);
	}

	sprintf(fname, "%s/.msh_history", getenv("HOME"));

	hist = fopen(fname, "r");
	if(hist == NULL) {
		/* do nothing */
	} else {
		while(getline(&buf, &len, hist) != -1) {
			histAdd(&(s->history), buf, NULL);
		}

		fclose(hist);
	}

	s->histfile = fopen(fname, "a");
	if(s->histfile == NULL) {
		error(ERR_FILE);
	}

	free(fname);

	s->running = 1;

	return;
}
