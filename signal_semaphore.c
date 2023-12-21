#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

// GLOBAL variables
#define SEM_NAME "/shazebs_semaphore"

sem_t semaphore;

void* parent_thread();

int main()
{
  sem_init(&semaphore, 1, 1);
  //sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);
  // if (sem == SEM_FAILED) {
  //   perror("sem_open fail\n");
  //   exit(EXIT_FAILURE);
  // }

  pid_t pid = fork();

  if (pid == -1) {
    perror("fork() fail\n");
    exit(EXIT_FAILURE);
  }
  else if (pid == 0)
  {
    printf("1\n");
    //sem_wait(sem);
    sem_wait(&semaphore);
    printf("2\n");
    sleep(60);
    printf("3\n");
    //sem_post(sem);
    sem_post(&semaphore);
    printf("4\n");
    //sem_close(sem);
    printf("5\n");
    exit(0);
  }
  else
  {
    pthread_t tid;
    void *tidResult;

    if (pthread_create(&tid, NULL, parent_thread, NULL) != 0)
    {
      perror("Failed to create thread\n");
      exit(EXIT_FAILURE);
    }

    // Wait for thread to finish.
    pthread_join(tid, &tidResult);

    if (tidResult == (void *)1)
    {
      printf("parent process failed to acquire semaphore, killing child process\n");
      kill(pid, SIGKILL);
      sleep(3);
      waitpid(pid, NULL, 0);
      printf("child process ended\n");
    }

    //if (sem_trywait(sem) == -1)
    if (sem_trywait(&semaphore) == -1)
    {
      printf("semaphore is still locked\n");
      sem_post(&semaphore);
      if (sem_trywait(sem) == 0)
      {
        printf("semaphore is finally retrieved\n");
      }
    }
    else
    {
      printf("Semaphore acquired by parent process\n");
      //sem_post(sem);
      sem_post(&semaphore);
    }

    //sem_close(sem);
    //sem_unlink(SEM_NAME); // cleanup: remove named semaphore
  }

  sem_destroy(&semaphore);
  printf("end of program\n");
  return 0;
}

void* parent_thread()
{
  // sem_t *sem = sem_open(SEM_NAME, 0);
  // if (sem == SEM_FAILED) {
  //   perror("sem_open failed in thread\n");
  //   return (void *)1;
  // }

  sleep(10);

  //if (sem_trywait(sem) == -1)
  if (sem_trywait(&semaphore) == -1)
  {
    printf("semaphore is still locked by child process\n");
    //sem_close(sem);
    return (void *)1;
  }

  printf("semaphore acquired by parent thread\n");
  //sem_post(sem);
  //sem_close(sem);
  sem_post(&semaphore);
  return (void *)0;
}
