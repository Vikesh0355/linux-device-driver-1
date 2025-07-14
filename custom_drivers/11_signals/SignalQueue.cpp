#include <iostream>
#include <csignal>
#include <unistd.h>
#include <cstdlib>

// Define a signal handler for the child process
void childHandler(int sig, siginfo_t* info, void* context) {
    std::cout << "Child process received signal " << sig << "\n";
    std::cout << "Signal data: " << info->si_value.sival_int << "\n";
}

int main() {
    pid_t pid = fork();  // Create a child process

    if (pid == 0) {  // Child process
        // Define a struct for the sigqueue signal handler
        struct sigaction action;
        action.sa_flags = SA_SIGINFO;  // Allow passing extra data with signal
        action.sa_sigaction = childHandler;  // Signal handler function

        // Set the signal handler for SIGUSR1
        if (sigaction(SIGUSR1, &action, NULL) == -1) {
            perror("sigaction failed");
            exit(EXIT_FAILURE);
        }

        std::cout << "Child process is waiting for a signal...\n";
        // Wait forever until the signal is received
        while (true) {
            sleep(1);  // Simulate doing work
        }

    } else if (pid > 0) {  // Parent process
        sleep(1);  // Ensure the child process is ready

        std::cout << "Parent process sending signal to child...\n";

        // Create a sigval union to pass data with the signal
        union sigval value;
        value.sival_int = 42;  // Send an integer value as the signal data

        // Send SIGUSR1 with the integer data to the child process
        if (sigqueue(pid, SIGUSR1, value) == -1) {
            perror("sigqueue failed");
            exit(EXIT_FAILURE);
        }

        // Wait a bit to ensure the signal is delivered
        sleep(1);

        // Terminate the child process
        kill(pid, SIGKILL);
        std::cout << "Parent process sent SIGKILL to terminate child.\n";
    } else {
        perror("Fork failed");
        return 1;
    }

    return 0;
}

/* Compile:
   g++ SignalQueue.cpp -o SignalQueue
   Run:
   ./SignalQueue
*/