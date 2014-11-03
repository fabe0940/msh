#include <stdlib.h>
#include "cmd.h"
#include "env.h"
#include "hist.h"
#include "status.h"

#include "clean.h"

void mshClean(status* s) {
	int i;
	cmdNode* cmd;
	envNode* env;
	histNode* hist;

	fclose(s->histfile);

	while(s->command != NULL) {
		cmd = s->command->next;
		free(s->command);
		s->command = cmd;
	}

	while(s->environment != NULL) {
		env = s->environment->next;
		free(s->environment);
		s->environment = env;
	}

	for(i = 0; i < 20; i++) {
		hist = s->history->next;
		free(s->history);
		s->history = hist;
	}
	
	return;
}
