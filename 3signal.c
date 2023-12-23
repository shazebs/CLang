#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/mman.h>

// Global Variables 
sem_t *semaphore;
pid_t otherPid;
sigset_t sigSet;

void signalHandler1(int signum)
{
    printf("Caught Signal: %d\n", signum);
    printf("    Exit Child Process.\n");
    sem_post(semaphore);
    _exit(0);
}

void signalHandler2(int signum)
{
    printf("I am alive.\n");
}

// Logic to run to simulate a Child Process
void childProcess()
{
    // Setup some Signal Handlers
    signal(SIGUSR1, signalHandler1);
    signal(SIGUSR2, signalHandler2);

    // Child Process: Simulates a hung process waiting on a semaphore (comment sem_post out) or running too long (comment sleep out)
    int value;
    sem_getvalue(semaphore, &value);
    printf("    Child Process: semaphore count is %d\n", value);
    printf("    Child Process: is grabbing semaphore.\n");
    sem_wait(semaphore);
    sem_getvalue(semaphore, &value);
    printf("    Child Process: semaphore count is %d\n", value);
    // *** Enter CRITICAL SECTION ***
    printf("    Starting very long Child Process....\n");
    for(int x=0; x < 60; ++x)
    {
        printf(".\n");
        sleep(1);
    }
    // *** End CRITICAL SECTION ***
    sem_post(semaphore);

    // Exit the Child Process (use _exit() and NOT exit()
    printf("    Exit Child Process\n");
    _exit(0);
}

void *checkHungChild(void *a)
{
    // Simulate a time of 10 seconds by going to sleep, then check if semaphore is locked indicating a hung Child Process
    int *status = a;
    printf("Checking for hung Child Process....\n");
    sleep(10);
    if (sem_trywait(semaphore) != 0)
    {
        printf("Child Process appears to be hung....\n");
        *status = 1;
    }
    else 
    {
        printf("Child Process appears to running fine....\n");
        *status = 0; 
    }
    return NULL;
}

// Logic to run to simulate a Parent Process
void parentProcess()
{
    // Parent process: Detect a hung Child Process and kill it after a timeout
    sleep(2);
    if (getpgid(otherPid) >= 0)
    {
        printf("Child Process is running...\n");
    }
    int value;
    sem_getvalue(semaphore, &value);
    printf("In the Parent Process with the semaphore count of %d.\n", value);

    // Try to get semaphore (or could have used sem_getvalue since sem_trywait would block) and if it is locked then start a timer
    if (sem_trywait(semaphore) != 0)
    {
        // Start Timer Thread and wait for it to return
        pthread_t tid;
        int status = 0;
        printf("Detected Child is hung or running too long...\n");
        if (pthread_create(&tid1, NULL, checkHungChild, &status))
        {
            printf("ERROR creating timer thread.\n");
            _exit(1);
        }
        if (pthread_join(tid1, NULL))
        {
            printf("\n ERROR joining timer thread.\n");
            exit(1);
        }

        if (status == 1)
        {
            // Kill Child Process 
            printf("Going to kill Child Process with ID of %d\n", otherPid);
            // kill(otherPid, SIGUSR1);
            kill(otherPid, SIGTERM);
            printf("Killed Child Process\n");

            // Prove that the Child Process is killed
            printf("Now Proving Child Process is killed (you should see no dots and no response from SIGUSR2 signal\n");
            sleep(5);
            kill(otherPid, SIGUSR2);
            sleep(1);
            printf("Done proving Child Process is killed\n");

            // Try to get semaphore
            sem_getvalue(semaphore, &value);
            printf("In the Parent Process with the semaphore count of %d.\n");
            if (sem_trywait(semaphore) != 0)
            {
                if (value == 0)
                    sem_post(semaphore);
                printf("Cleaned up and finally got the semaphore.\n");
                sem_getvalue(semaphore, &value);
                printf("In the Parent Process with the semaphore count of %d.\n", value);
            }
            else 
            {
                printf("Finally got the semaphore.\n");
            }
        }
    }

    // Exit the Parent Process (use _exit() and NOT exit()
    printf("Exit Parent Process.\n");
    _exit(0);
}

int main(int argc, char *argv[])
{
    pid_t pid;

    // Create shared semaphore
    semaphore = (sem_t*)mmap(0,sizeof(sem_t), PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    if (sem_init(semaphore, 1, 1) != 0)
    {
        printf("Failed to create semaphore.\n");
        exit(EXIT_FAILURE);
    }

    // Use fork()
    pid = fork();
    if (pid == -1)
    {
        printf("Can't fork, error.\n");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        printf("    Started Child Process with Process ID of %d\n", getpid());
        otherPid = getppid();
        childProcess();
    }
    else 
    {
        // run Parent Process logic
        printf("Started Parent Process with Process ID of %d...\n", getpid());
        otherPid = pid;
        parentProcess();
    }

    // Cleanup
    sem_destroy(semaphore);

    // Return OK
    return 0;
}
