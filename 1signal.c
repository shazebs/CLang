#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <errno.h>

// Constants
int MAX = 100;
int WAKEUP = SIGUSR1;
int SLEEP = SIGUSR2;

pid_t otherPid;

// Signal Set
sigset_t sigSet;

// Shared Circular Buffer
struct CIRCULAR_BUFFER
{
    int count;
    int lower;
    int upper;
    int buffer[MAX];
};
struct CIRCULAR_BUFFER *buffer = NULL;

// This method will put the current Process to sleep forever until it is awoken by the WAKEUP signal.
void sleepAndWait()
{
    int nSig;

    // Sleep until notified to wakeup
    printf("Sleeping.....\n");
    sigwait(&sigSet, &nSig);
    printf("Awoken.\n");
}

// This method will signal Other Process to WAKEUP.
void wakeupOther()
{
    kill(otherPid, WAKEUP);
}

// Gets a value from the shared buffer.
int getValue()
{
    // Get a value from the Buffer and adjust where to read from next.
    int value = buffer->buffer[buffer->lower];
    printf("    Consumer read value of %c\n", value);
    ++buffer->lower;
    if(buffer->lower == MAX)
    {
        buffer->lower = 0;
    }
    --buffer->count;
    return value;
}

// Puts a value in the shared buffer.
void putValue(int value)
{
    buffer->buffer[buffer->upper] = value;
    printf("Producer stored value of %c\n", buffer->buffer[buffer->upper]);
    ++buffer->upper;
    if(buffer->upper == MAX)
        buffer->upper = 0;
    ++buffer->count;
}

// Logic to run Consumer Process.
void consumer()
{
    // Setup a Signal Set
    sigemptyset(&sigSet);
    sigaddset(&sigSet, WAKEUP);
    sigprocmask(SIG_BLOCK, &sigSet, NULL);

    // Run the Child Consumer logic.
    printf("Running the Child Consumer Process...\n");

    // Reads characters from shared buffer until a 0 is received.
    int character = 0;
    do 
    {
        // Wait to be notified there is a character in the shared buffer.
        sleepAndWait();

        // Read the character from the shared buffer until 0 has been received.
        character = getValue();
    } while (character != 0); 

    // Exit cleanly from the Consumer Process
    printf("Exiting the Child Consumer Process....\n");
    _exit(1);
}

// Logic to run Producer Process.
void producer()
{
    // Buffer value to write
    int value = 0; 

    // Run the Parent Producer logic
    printf("Running the Parent Producer Process...\n");

    // Send a desired message to the Child Consumer Process
    char message[13] = "Shazeb Suhail";
    for(int x = 0; x < strlen(message); x++)
    {
        // Puts the next character in the shared buffer, notify the consumer, sleep a bit.
        putValue(message[x]);
        wakeUpOther();
        sleep(1);
    }
    // The the child consumer process that all characters have been sent.
    putValue(0);
    wakeupOther();

    // Exit cleanly from the Producer Process
    printf("Exiting the Parent Producer Process...\n");
    _exit(1);
}

int main(int argc, char* argv[])
{
    pid_t pid;

    // Create a shared memory for the Circular Buffer to be shared between the Parent and Child Processes.
    buffer = (struct CIRCULAR_BUFFER*)mmap(0,sizeof(buffer), PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    buffer->count = 0;
    buffer->lower = 0;
    buffer->upper = 0;

    // Use fork()
    pid = fork();
    if (pid == -1)
    {
        // Error
        printf("Can't fork, error %d\n", errno);
        exit(EXIT_FAILURE);
    }
    // OK
    if (pid == 0)
    {
        // Run Producer Process logic as a Child Process.
        otherPid = getppid();
        producer();
    }
    else
    {
        // Run Consumer Process logic as a Parent Process.
        otherPid = pid;
        consumer();
    }

    // Return OK
    return 0; 
}