/*
Author: Shazeb Suhail
Date: Sun.Nov.26.2023
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define LOOP_COUNT 17
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;
int misses = 0;

void* counter_thread();
void* monitor_thread();

void main()
{
  pthread_t tid1, tid2;
  pthread_create(&tid1, NULL, counter_thread, NULL);
  pthread_create(&tid2, NULL, monitor_thread, NULL);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  printf("---------------\nCounter = %d\n", counter);
  printf("Misses = %d\n", misses);
  pthread_mutex_destroy(&mutex);
}

void* counter_thread()
{
  for (int i=0; i<LOOP_COUNT; i++)
  {
    sleep(1);
    pthread_mutex_lock(&mutex);
    counter++;
    sleep(1);
    pthread_mutex_unlock(&mutex);
  }
  pthread_exit(NULL);
}

void* monitor_thread()
{
  while (1)
  {
    sleep(3);
    if (pthread_mutex_trylock(&mutex) != 0)
    {
      misses++;
    }
    else
    {
      printf("counter = %d\n", counter);
      pthread_mutex_unlock(&mutex);
    }

    if (counter == LOOP_COUNT) break;
  }
  pthread_exit(NULL);
}
