#ifndef _LIST_H_
#define _LIST_H_

struct node {
	char ch;
	struct node* next;
};

void listAdd(struct node* head, char newch);
void listFree(struct node* head);
char* listToString(struct node* head);

#endif
