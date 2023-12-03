#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <time.h>

#define TIMEOUT 5 // seconds

// Simulate resource access
bool isResourceAvailable() {
    // Simulate resource availability
    // In a real scenario, this would involve actual resource management
    return rand() % 2 == 0;
}

int main() {
    srand(time(NULL)); // Initialize random number generator

    pid_t pid = fork();

    if (pid == -1) {
        // Fork failed
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        time_t start = time(NULL);

        while (true) {
            if (isResourceAvailable()) {
                fprintf(stdout, "Child process %d accessed the resource\n", getpid());
                break;
            } else {
                if (difftime(time(NULL), start) > TIMEOUT) {
                    fprintf(stdout, "Child process %d starved and terminated\n", getpid());
                    return 0; // terminate process
                }
            }
        }
    } else {
        // Parent process
        fprintf(stdout, "Parent process %d running\n", getpid());
        wait(NULL); // Wait for child process to finish
    }

    return 0;
}
