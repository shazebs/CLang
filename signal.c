#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

void wakeup_handler(int num){
  printf("****SIGNAL RECEIVED BY CHILD PROCESS****\n");
  for (int c=1; c<=20; c++){
    printf("---------------[CHILD=%d]\n", c);
    sleep(1);
  }
}

void main(void){
  pid_t pid = fork();
  if (pid==0){
    signal(SIGUSR1, wakeup_handler); // signal handler.
    pause(); // wait for signal to continue.
    exit(1);
  }
  else if (pid>0){
    printf("**{PARENT_PID=%d}**\n", getpid());
    printf("**{CHILD_PID=%d}**\n", pid);
    for (int p=1; p<=30; p++){
      if (p==5){
        printf("****SENDING SIGNAL TO CHILD PROCESS****\n");
        kill(pid, SIGUSR1);
      }
      printf("[PARENT=%d]\n", p);
      sleep(1);
    }
    wait(NULL);   // wait for child process to end.
    exit(1);
  }
  else {
    perror("****fork() failed****");
    exit(EXIT_FAILURE);
  }
}
/*
Author: Shazeb Suhail
Date: Sat.Nov.18.2023
*/
