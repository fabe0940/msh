#ifndef ENV_H
#define ENV_H

struct _envVar {
	char* name;
	char* val;
};
typedef struct _envVar envVar;

struct _envNode {
	envVar* var;
	struct _envNode* next;
};
typedef struct _envNode envNode;

#endif
