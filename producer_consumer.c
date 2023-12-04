/*
Author: Shazeb Suhail
Date: Fri.Nov.24.2023
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>

// GLOBAL variables
#define MAX_LOOPS 10
#define BUFFER_EOF 11

pid_t pid;
pid_t other_pid;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// STRUCTURE definitions
struct CIRCULAR_BUFFER
{
  int count;        // number of items in the buffer
  int lower;        // next slot to read in buffer
  int upper;        // next slot to write in buffer
  int buffer[MAX_LOOPS];
};
struct CIRCULAR_BUFFER *buffer = NULL;

// PROTOTYPE functions
void producer();
void consumer();

// DRIVER method
int main(){
  srand(time(NULL));
  // CREATE BUFFER
  buffer = (struct CIRCULAR_BUFFER*)mmap(0,sizeof(buffer), PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS, -1, 0);
  buffer->count = 0;
  buffer->lower = 0;
  buffer->upper = 0;

  // begin fork()
  pid = fork();
  if (pid == -1){
    printf("Can't fork, error %d\n", errno);
    exit(EXIT_FAILURE);
  }
  // CHILD PROCESS
  if (pid == 0)
  {
    printf("CREATED PRODUCER()\n");
    other_pid = getppid(); // get parent pid
    while(1){
      printf("producer: waiting for signal...\n");
      signal(SIGUSR1, producer);
      sleep(2);
    }
  }
  // PARENT PROCESS
  else
  {
    printf("CREATED CONSUMER()\n");
    other_pid = pid;
    while(1){
      sleep(1);
      printf("consumer: sending signal to Producer | other_pid = %d\n", (int)other_pid);
      kill(other_pid, SIGUSR1);

      printf("consumer: waiting for signal...\n");
      signal(SIGUSR1, consumer);
      pause();

      buffer->count = 0;
      buffer->lower = 0;
      buffer->upper = 0;
    }
  }
  // END PROGRAM
  pthread_mutex_destroy(&mutex);
  return 0;
}

/* Create args to 'pass' to Consumer. */
void producer()
{
  printf("PRODUCER: received signal!\n\n");
  int loop_count = 0;
  while (loop_count < MAX_LOOPS)
  {
    sleep(1);
    int rand_num = rand() % 101;
    pthread_mutex_lock(&mutex);
    if (buffer->count < MAX_LOOPS)
    {
      buffer->count = buffer->count + 1;        // increment count
      buffer->buffer[buffer->upper] = rand_num; // set current buffer[upper] value a random number
      int prevUpper = buffer->upper;            // set a temporary current uppper index value
      buffer->upper = buffer->upper + 1;        // increment buffer index value
      printf("PRODUCER: buffer[%d]=%d\n", prevUpper, rand_num);

      // exit signal handler
      if (buffer->upper == BUFFER_EOF){
        return;
      }
      // send signal
      if (buffer->lower == 0 && buffer->count > 0)
      {
        sleep(1);
        kill(other_pid, SIGUSR1); // wakeup Consumer process
      }
    }
    pthread_mutex_unlock(&mutex);
    loop_count++;
  }
  printf("PRODUCER: done writing, buffer is full.\n");
}

/* Read 'passed' args from Producer */
void consumer()
{
  printf("CONSUMER: received signal!\n");
  int count = 0;
  while (count < MAX_LOOPS)
  {
    sleep(1);
    pthread_mutex_lock(&mutex);
    if (buffer->count > 0)
    {
      if (buffer->lower < buffer->upper)
      {
        printf("\tCONSUMER: buffer[%d]=%d\n\n", buffer->lower, buffer->buffer[buffer->lower]);
        buffer->lower++;
        if (buffer->lower == BUFFER_EOF)
        {
          return;
        }
      }
    }
    pthread_mutex_unlock(&mutex);
    count++;
  }
  printf("CONSUMER: done reading, buffer is empty.\n\n");
}
