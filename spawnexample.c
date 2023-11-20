#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <spawn.h>

extern char **environ;

int main(int argc, char *argv[]){
  if (argc < 2){
    fprintf(stderr, "Usage :%s <path-to-executable>\n", argv[0]);
    return EXIT_FAILURE;
  }  
  pid_t pid;
  int status;
  char cmd[100] = "/bin/";
  strncat(cmd, argv[1], 95);
  char *args[]={cmd, NULL};
  
  if (posix_spawn(&pid, cmd, NULL, NULL, args, environ) != 0){
    printf("posix_spawn failed to execute command=%s\n", cmd);
    return EXIT_FAILURE;
  }
  sleep(1);
  printf("Spawned %s command with PID=%d\n", cmd, pid);
  
  if (waitpid(pid, &status, 0) == -1){
    perror("waitpid failed");
    return EXIT_FAILURE;
  }
  
  if (WIFEXITED(status)){
    printf("Process exited with status=%d\n", WEXITSTATUS(status));
  } else {
    printf("Process terminated abnormally.");
  }
}
/*
Author: Shazeb Suhail
Date: Thu.Nov.16.2023
*/
