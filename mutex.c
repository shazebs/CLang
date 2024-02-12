/*
Author: Shazeb Suhail
Date: Sun.Feb.11.2024
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#define NUM_LOOPS 10000000   // 10 million
pthread_mutex_t mutex;  // = PTHREAD_MUTEX_INITIALIZER;
long long count = 0;

void* increment_thread();
void* decrement_thread();

void main(void){
  pthread_mutex_init(&mutex, 0);
  pthread_t P1,P2;

  // CREATE THREADS
  pthread_create(&P1, NULL, &increment_thread, NULL);
  pthread_create(&P2, NULL, &decrement_thread, NULL);

  // WAIT ON THREADS
  pthread_join(P1, NULL);
  pthread_join(P2, NULL);

  // END PROGRAM
  printf("Count=%lld\n", count); // print output
  pthread_mutex_destroy(&mutex);
}

/* Method for Incrementing count value by 1 */
void* increment_thread()
{
  for (int i = 0; i < NUM_LOOPS; i++) {
    // ** START CRITICAL SECTION **
    pthread_mutex_lock(&mutex);
    int temp = count + 1;
    count = temp;
    pthread_mutex_unlock(&mutex);
    // ** END CRITICAL SECTION **
  }
  pthread_exit(NULL);
}

/* Method for Decrementing count value by 1 */
void* decrement_thread()
{
  for (int i = 0; i < NUM_LOOPS; i++) {
    // ** START CRITICAL SECTION **
    pthread_mutex_lock(&mutex);
    int temp = count - 1;
    count = temp;
    pthread_mutex_unlock(&mutex);
    // ** END CRITICAL SECTION **
  }
  pthread_exit(NULL);
}
