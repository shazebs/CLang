/*
Author: Shazeb Suhail
Date: Sun.Feb.11.2024
*/
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// GLOBAL VARIABLES
#define NUM_TOILETS 3
#define NUM_USERS 13
#define MIN_TIME 2
#define MAX_TIME 9
sem_t semaphore;

// PROTOTYPE functions
void* user_thread(void* arg);

// DRIVER function
void main(void)
{
  srand(time(NULL));
  sem_init(&semaphore, 0, NUM_TOILETS);
  pthread_t threads[NUM_USERS];

  // RUN THREADS
  for (int i=0; i<NUM_USERS; i++){
    int* id = malloc(sizeof(int));
    *id = i;
    pthread_create(&threads[i], NULL, &user_thread, id);
  }
  // WAIT ON THREADS
  for (int i=0; i<NUM_USERS; i++)
  {
    pthread_join(threads[i], NULL);
  }
  sem_destroy(&semaphore);
}

/* Occupy a toilet thread for a random time in seconds. */
void* user_thread(void* arg)
{
  int id = *(int*)arg;
  printf("** User (%d) is busy waiting for a toilet **\n", id);
  // ** START CRITICAL SECTION **
  //
  sem_wait(&semaphore);
  printf("O<-----User (%d) got access to a toilet!\n\n", id);
  int rand_time = MIN_TIME + rand() % (MAX_TIME - MIN_TIME + 1);
  for (int i=0; i<rand_time; i++)
  {
    sleep(1); printf(".\n");
  }
  printf("\nX----->User (%d) exited the toilet after %d seconds.\n\n", id, rand_time);
  sem_post(&semaphore);
  //
  // ** END CRITICAL SECTION **
  free(arg);
  pthread_exit(NULL);
}
