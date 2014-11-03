#ifndef EVAL_H
#define EVAL_H

#include "status.h"

void mshEval(status* s);
int evalCompare(listNode* l, char* str);
void evalSplit(cmdNode** head, listNode* l, char div);
void evalReplace(listNode** l, char* find, char* repl);


#endif
