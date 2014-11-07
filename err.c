#include <stdlib.h>
#include <stdio.h>

#include "err.h"

/* print an error message and exit */
void error(const char* msg) {
	fprintf(stderr, "%s\n", msg);
	exit(1);
}
