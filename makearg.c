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
	char* str;
	char* end;

	argc = 0;

	/* skip leading whitespace */
	str = s;
	while(isWhitespace(*str) && *str != '\0') {
		str++;
	}

	/* count words */
	if(strlen(str) != 0) {
		ch = str;
		while(ch < ((strlen(str) + str + 1))) {
			argc += isWhitespace(*ch);

			for(; isWhitespace(*ch); ch++) {
				/* Do Nothing */
			}

			ch++;
		}
	} else {
		argc = 0;
	}

	/* allocate array of strings */
	*args = (char**) malloc((argc * (sizeof(char*)) + 1));
	if(*args == NULL) goto fail;

	ch = str;
	end = str;

	/* populate array of strings one word at a time */
	for(i = 0; i < argc; i++) {
		/* find the end of a word */
		for(end = ch; !isWhitespace(*end); end++) {
			/* Do Nothing */
		}

		/* mark the end */
		*end = '\0';

		/* create new string */
		*((*args) + i) = malloc((strlen(ch) + 1) * sizeof(char));
		if(*((*args) + i) == NULL) goto fail;

		/* copy word into string */
		strcpy(*((*args) + i), ch);

		/* move to the start of the next word */
		for(ch = end; isWhitespace(*ch); ch++) {
			/* Do Nothing */
		}
	}

	/* NULL terminate the array */
	(*((*args) + (argc))) = NULL; 

	return argc;

	fail:
		return -1;
}
