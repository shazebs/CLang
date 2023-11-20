#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void ParentProcess(void);
void ChildProcess(void);

void main(void){
  pid_t pid = fork();
  if(pid==0){
    ChildProcess();
  }
  else if (pid>0) {
    ParentProcess();
  }
  exit(0);
}

void ParentProcess(void){
  for(int i=1; i<=10; i++){
    printf("Parent line %d\n", i);
    sleep(2);
  }
}

void ChildProcess(void){
  for(int i=1; i<=10; i++){
    printf("   Child line %d\n", i);
    sleep(1);
  }
}
