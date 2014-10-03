#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "list.h"
#include "makearg.h"
int main(int argc, char** argv) {
  /* variables */
  char buff;
	char* str;
  char** argVals;
  int argNum;
  int i;
  int running;
  int status;
  pid_t pid;
  struct node* head;

  /* process arguments */

  /* initialize */
  argNum = 0;
  running = 1;
  head = NULL;
  argVals = NULL;

  /* main loop */
  while(running) {

    /* get input */
    fprintf(stdout, "?: ");
    do {
      buff = (char) getchar();
      listAdd(&head, buff);
    } while(buff != '\n');

    
    /* parse input */


	str = listToString(head);
    
    argNum = makearg(str, &argVals);

	free(str);
	str = NULL;
    listFree(&head);
    head = NULL;

    /* builtins */
    if(!strcmp(argVals[0], "exit")) {
      running = 0;
      break;
    }

    /* fork */
    pid = fork();
    if(pid == -1) {

      /* error */
      fprintf(stderr, "msh: unable to fork.\n");
      break;
    } else if(pid == 0) {

      /* child -- exec command */
      if(execvp(argVals[0], argVals) == -1) {
	fprintf(stderr, "msh: unable to execute %s\n", argVals[0]);
	exit(1);
      }
      exit(0);
    } else if(pid > 0) {

      /* parent -- wait for child */
      wait(&status);
    }
  for(i = 0; i < argNum; i++) {
    free(argVals[i]);
  }
  free(argVals);
    
  }

  /* Clean Up */ 
  exit(0);
}
