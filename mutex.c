#include <stdio.h>
#include <pthread.h>

#define NUM_LOOPS 50000000 // 10 million
long long bankbalance = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* deposits_thread(){
  for (int i=0; i<NUM_LOOPS; i++){
    pthread_mutex_lock(&mutex);
    bankbalance += 1;
    pthread_mutex_unlock(&mutex);
  }
  pthread_exit(NULL);
}

int main(void){
  //pthread_mutex_init(&mutex, 0);
  pthread_t tid1;
  pthread_create(&tid1, NULL, deposits_thread, NULL);

  pthread_t tid2;
  int limit2;
  pthread_create(&tid2, NULL, deposits_thread, NULL);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  pthread_mutex_destroy(&mutex);
  printf("Final bank balance = $%lld\n", bankbalance);
  return 0;
}
/*
Author: Shazeb Suhail
Date: Sun.Nov.19.2023
*/
