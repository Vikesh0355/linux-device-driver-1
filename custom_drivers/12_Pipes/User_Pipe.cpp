#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>

int main() {
    int pipefd[2];
    pid_t pid;
    const char* message = "Hello, child process!";
    char buffer[100];

    // Create the pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid = fork();  // Create a child process

    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {  // Child process
        close(pipefd[1]);  // Close the write end of the pipe (not needed by child)

        // Read data from pipe
        read(pipefd[0], buffer, sizeof(buffer));
        std::cout << "Child received message: " << buffer << std::endl;

        close(pipefd[0]);
    } else {  // Parent process
        close(pipefd[0]);  // Close the read end of the pipe (not needed by parent)

        // Write data to pipe
        write(pipefd[1], message, strlen(message) + 1);  // Including the null terminator
        std::cout << "Parent sent message: " << message << std::endl;

        close(pipefd[1]);
    }

    return 0;
}


/* Compile:
   g++ User_Pipe.cpp -o User_Pipe
   Run:
   ./User_Pipe
*/