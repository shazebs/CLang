#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <errno.h>

pthread_mutex_t mutex;
int end_time; 
int time;

// 
void *counter_thread(void *arg)
{
    int status;

    // For the specified time duration increment the counter;
    while (time (NULL) < end_time)
    {
        // *** Enter CRITICAL SECTION ***
        status = pthread_mutex_lock(&mutex); 
        if (status == 0)
            printf("Counter Thread: Locked mutex for Counter so it can be incremented.\n");
        ++counter;

        sleep(1); // sleep hog for a greater chance of trylock failing

        status = pthread_mutex_unlock(&mutex);
        if (status == 0)
            printf("Counter Thread: Unlocked mutex for Counter.\n");
        // *** End CRITICAL SECTION ***  
    }

    // Print the final value of the Counter
    printf("Final Counter is %lu\n", counter);
    return NULL;
}

// The monitor thread simply tries to get the mutex and if locked keeps track of the Misses count else accesses the Counter.
void *monitor_thread(void *arg)
{
    int status;
    int misses = 0;

    // For the specified time duration try to get the mutex and if locked count it else access the counter and print it
    while (time (NULL) < end_time)
    {
        // Try to get the mutex and if not busy access the Counter to print it else keep track of misses
        status = pthread_mutex_trylock(&mutex);
        if (status != EBUSY)
        {
            printf("    Monitor Thread: the trylock will lock the mutex so we can safely read the Counter\n");
            printf("    Monitor Thread: the Counter from Monitor is %ld\n", counter);
            status = pthread_mutex_unlock(&mutex);
            if (status == 0)
                printf("    Monitor Thread: Mutex unlocked, done with Counter.");
        }
        else 
        {
            // Count Misses on the lock
            ++misses;
        }
    }

    // Print number of Misses on the lock.
    printf("Final Monitor Thread missed was %d times.\n", misses);
    return NULL;
}

int main(int argc, char *argv[])
{
    int status;
    pthread_t counter_thread_id;
    pthread_t monitor_thread_id;

    // Initialize the mutex
    pthread_mutex_init(&mutex, 0);

    // Set the end time for 20 seconds from now.
    end_time = time (NULL) + 20;

    // Create the Counter and Monitor threads.
    if (pthread_create(&counter_thread_id, NULL, counter_thread, NULL))
        printf("Create counter thread failed.\n");
    if (pthread_create(&monitor_thread_id, NULL, monitor_thread, NULL))
        printf("Create monitor thread failed.\n");

    // Wait for Counter and Monitor threads to both finish
    if (pthread_join(counter_thread_id, NULL))
        printf("Joined counter thread failed.\n");
    if (pthread_join(monitor_thread_id, NULL))
        printf("Joined monitor thread failed.\n");

    return 0;
}
