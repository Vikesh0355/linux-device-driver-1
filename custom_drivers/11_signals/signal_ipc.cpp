#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <csignal>
#include <cstdlib>

// Signal handler for the child process
void childHandler(int signum) {
    std::cout << "Child process received signal " << signum << "\n";
}

int main() {
    // Create a child process
    pid_t pid = fork();

    if (pid == 0) {  // Child process
        // Register signal handler for SIGUSR1
        signal(SIGUSR1, childHandler);
        
        // Infinite loop, waiting for signals
        std::cout << "Child process started. Waiting for signal...\n";
        while (true) {
            // Simulating some work in the child process
            sleep(1);
        }

    } else if (pid > 0) {  // Parent process
        std::cout << "Parent process started. Sending signal to child...\n";
        
        // Wait for a while before sending the signal
        sleep(2);

        // Send SIGUSR1 signal to child
        kill(pid, SIGUSR1);

        // Wait for the child process to handle the signal
        sleep(2);

        // Terminate child process
        kill(pid, SIGKILL);
        std::cout << "Parent sent SIGKILL to child. Exiting...\n";
    } else {
        std::cerr << "Fork failed.\n";
        return 1;
    }

    return 0;
}

/* Compile:
   g++ signal_ipc.cpp -o signal_ipc
   Run:
   ./signal_ipc
*/