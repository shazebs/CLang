#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_LOOPS 50000000 // 50 million
long long bankbalance = 0;
sem_t semaphore;

void* deposits_thread(){
  int i = 0;
  while (i < NUM_LOOPS){
    sem_wait(&semaphore);
    bankbalance += 1;
    sem_post(&semaphore);
    i++;
  }
  pthread_exit(NULL);
}

int main(void){
  sem_init(&semaphore, 0, 1);
  pthread_t tid1;
  pthread_create(&tid1, NULL, deposits_thread, NULL);
  pthread_t tid2;
  int limit2;
  pthread_create(&tid2, NULL, deposits_thread, NULL);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  sem_destroy(&semaphore);
  printf("Final bank balance = $%lld\n", bankbalance);
  return 0;
}
/*
Author: Shazeb Suhail
Date: Sun.Nov.19.2023
*/
