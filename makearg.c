#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "makearg.h"

/* check if a character is whitespace */
int isWhitespace(char ch) {
	int res;

	/* ' ', '\t', '\n', and '\0' are all considered whitespace, as calling
	   '\0' whitespace makes the argc loop in makearg() cleaner */
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

	argc = 0;

	/* count words, where a "word" is a sequence of characters of nonzero
	   length followed by whitespace */
	for(ch = s; ch < (strlen(s) + s); ch++) {
		/* add one to argc when whitespace is found */
		argc += isWhitespace(*ch);

		/* skip th rest of the whitespace */
		for(; isWhitespace(*ch); ch++) {
			/* Do Nothing */
		}
	}

	/* allocate an array of strings one larger than the number of arguments,
	   so that there is room for the final NULL */
	*args = (char**) malloc((argc * (sizeof(char*)) + 1));
	if(*args == NULL) goto fail;

	ch = s;
	end = s;

	/* copy each argument from s into args */
	for(i = 0; i < argc; i++) {
		/* move end to the end of the current word */
		for(end = ch; !isWhitespace(*end); end++) {
			/* Do Nothing */
		}

		/* mark the end of the word with '\0' */
		*end = '\0';

		/* allocate a new string inside of args long enough for the string and a
		   final '\0' */
		*((*args) + i) = malloc((strlen(ch) + 1) * sizeof(char));
		if(*((*args) + i) == NULL) goto fail;

		/* copy the string from s to args */
		strcpy(*((*args) + i), ch);

		/* move the start of the word to the first non-whitespace character past
		   end */
		for(ch = end; isWhitespace(*ch); ch++) {
			/* Do Nothing */
		}
	}

	/* NULL terminate args */
	(*((*args) + (argc))) = NULL; 

	return argc;

	/* return an error code */
	fail:
		return -1;
}
