#ifndef LIST_H
#define LIST_H

struct _listNode {
	char ch;
	struct _listNode* next;
};
typedef struct _listNode listNode;

void listAdd(listNode** head, char newch);
void listFree(listNode** head);
void listPrint(listNode** head);
char* listToString(listNode* head);

#endif
