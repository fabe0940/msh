#include <stdlib.h>
#include "cmd.h"
#include "status.h"

#include "update.h"

/* between command maintenance */
void mshUpdate(status* s) {
	cmdNode* cmd;

	/* free old command nodes */
	while(s->command != NULL) {
		cmd = s->command->next;
		free(s->command);
		s->command = cmd;
	}
	s->command = NULL;
	
	return;
}
