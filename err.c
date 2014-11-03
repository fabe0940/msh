#include <stdlib.h>
#include <stdio.h>

#include "err.h"

void error(const char* msg) {
	fprintf(stderr, "%s\n", msg);
	exit(1);
}
