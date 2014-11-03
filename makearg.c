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

	str = s;
	while(isWhitespace(*str) && *str != '\0') {
		str++;
	}

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

	*args = (char**) malloc((argc * (sizeof(char*)) + 1));
	if(*args == NULL) goto fail;

	ch = str;
	end = str;

	for(i = 0; i < argc; i++) {
		for(end = ch; !isWhitespace(*end); end++) {
			/* Do Nothing */
		}

		*end = '\0';

		*((*args) + i) = malloc((strlen(ch) + 1) * sizeof(char));
		if(*((*args) + i) == NULL) goto fail;

		strcpy(*((*args) + i), ch);

		for(ch = end; isWhitespace(*ch); ch++) {
			/* Do Nothing */
		}
	}

	(*((*args) + (argc))) = NULL; 

	return argc;

	fail:
		return -1;
}
