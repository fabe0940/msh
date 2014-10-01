#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "makearg.h"

int isWhitespace(char ch) {
	int res;

	switch(ch) {
		case ' ':
		case '\t':
		case '\n':
		case '\0':
			res = 1;
			break;
		default:
			res = 0;
	}

	return res;
}

int makearg(char* s, char*** args) {
	int i;
	int argc;
	char* ch;
	char* end;

	/* count words */

	argc = 0;
	for(ch = s; ch < (strlen(s) + s); ch++) {
		argc += isWhitespace(*ch);

		for(; isWhitespace(*ch); ch++) {
			/* Do Nothing */
		}
	}

	/* parse words */

	*args = (char**) malloc(argc * sizeof(char*));
	if(*args == NULL) goto fail;


	ch = s;
	end = s;
	for(i = 0; i < argc; i++) {
		for(end = ch; !isWhitespace(*end); end++) {
			/* Do Nothing */
		}
		*end = '\0';

		*((*args) + i) = malloc((strlen(ch) + 1) * sizeof(char));
		if(*((*args) + i) == NULL) goto fail;

		strcpy(*((*args) + i), ch);

		for(ch = (end + 1); isWhitespace(*ch); ch++) {
			/* Do Nothing */
		}
	}

	return argc;

	fail:
	fprintf(stderr, "FAIL\n");
	return -1;
}
