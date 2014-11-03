#include <stdlib.h>
#include "cmd.h"
#include "status.h"

#include "update.h"

void mshUpdate(status* s) {
	cmdNode* cmd;

	while(s->command != NULL) {
		cmd = s->command->next;
		free(s->command);
		s->command = cmd;
	}
	s->command = NULL;
	
	return;
}
