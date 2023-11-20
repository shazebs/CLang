#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <spawn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

void run_cmd(char cmd[]){
  pid_t pid;
  int status;
  char path[]="/bin/";
  int pathlen=(int)strlen(path);
  int cmdlen=(int)strlen(cmd);
  int pathcmdlen=pathlen+cmdlen;
  char pathcmd[pathcmdlen];
  strncat(pathcmd, path, pathlen);
  strncat(pathcmd, cmd, cmdlen);
  char *args[]={pathcmd, NULL};

  printf("...Running command: %s\n", pathcmd);
  status = posix_spawn(&pid, pathcmd, NULL, NULL, args, environ);
  if (status==0){
    printf("...Child pid=%i\n", pid);
    do{
      if (waitpid(pid, &status, 0) != -1){
        printf("...Child status=%d\n", WEXITSTATUS(status));

      } else {
        perror("...waitpid");
        exit(9);
      }
    } while (!WIFEXITED(status));
  } else {
    printf("...posix_spawn failure: %s\n", strerror(status));
  }
}
int main(int argc, char *argv[]){
  if (argc < 2){
    fprintf(stderr, "...Error, enter a command after %s\n", argv[0]);
    return EXIT_FAILURE;
  }
  run_cmd(argv[1]);
  return 0;
}
/*
Author: Shazeb Suhail
Date: Fri.Nov.17.2023

References:
Ma, E. (2021, Aug 28). "Creating a Child Process using posix_spawn in C in Linux". SysTutorials. https://www.systutorials.com/a-posix_spawn-example-in-c-to-create-child-process-on-linux/. [Access: Nov 17, 2023].
*/
