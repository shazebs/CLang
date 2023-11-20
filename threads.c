#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>

void* thread_1(void* arg){
  for (int i=1; i<=10; i++){
    printf("[THREAD_1=%d]\n", i);
    sleep(1);
  }
  pthread_exit(NULL);
}

void* thread_2(void* arg){
  for (int i=1; i<=10; i++){
    printf("---------------[THREAD_2=%d]\n", i);
    sleep(2);
  }
  pthread_exit(NULL);
}

void main(int argc, char **argv){
  pthread_t tids[2];
  pthread_attr_t attr;
  pthread_attr_init(&attr);  
  int tresults[2];
  tresults[0] = pthread_create(&tids[0], &attr, thread_1, NULL);  // start thread 1
  tresults[1] = pthread_create(&tids[1], &attr, thread_2, NULL);  // start thread 2
  
  for (int t=0; t < 2; t++){
    pthread_join(tids[t], NULL);  // wait for thread to end.
    printf("pthread tid=%d has ended with result=%d.\n", (int)tids[t], tresults[t]);
  }
}
