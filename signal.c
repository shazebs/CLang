#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>

// GLOBAL variables
#define STRING_SIZE 100
char string[] = "Shazeb Suhail\n";
pid_t pid;
int loop = 1;

// STRUCTURE definitions
struct BUFFER
{
  int index;
  int buffer[STRING_SIZE];
};
struct BUFFER *buffer = NULL;

// PROTOTYPE functions
void child_process();
void parent_process();

// DRIVER method
int main()
{
  buffer = (struct BUFFER*)mmap(0,sizeof(buffer), PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS, -1, 0);
  buffer->index = 0;

  pid = fork();
  // CHILD PROCESS
  if (pid == 0)
  {
    sleep(2);
    int parent_pid = getppid();
    while (loop == 1)
    {
      sleep(1);
      signal(SIGUSR1, child_process);

      if (loop == 0) {
        break;
      }

      kill(pid, SIGUSR1);
    }
  }
  // PARENT PROCESS
  else
  {
    while (loop == 1)
    {
      signal(SIGUSR1, parent_process);
      pause();

      sleep(2);
      kill(pid, SIGUSR1);

      if (loop == 0) {
        break;
      }
    }
  }
  // END PROGRAM
  return 0;
}

void parent_process()
{
  printf("\n");
  if (string[buffer->index] == '\n')
  {
    buffer->buffer[buffer->index] = string[buffer->index];
    buffer->index++;
    loop = 0;
  }
  else
  {
    printf("<----- P: string[%d]='%c'\n", buffer->index, string[buffer->index]);
    buffer->buffer[buffer->index] = string[buffer->index];
    buffer->index++;
  }
}

void child_process()
{
  if (buffer->buffer[buffer->index-1] != '\n')
  {
    printf("-----> C: buffer[%d]='%c'\n", buffer->index-1, buffer->buffer[buffer->index-1]);
  }
  else
  {
    printf("End of buffer!\n");
    loop = 0;
    return;
  }
}
