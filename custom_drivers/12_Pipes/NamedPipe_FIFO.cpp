#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

int main() {
    const char *fifo_path = "/tmp/myfifo";
    char message[] = "Hello from the parent process!";
    char buffer[100];
    int fd;

    // Create the FIFO if it does not exist
    if (mkfifo(fifo_path, 0666) == -1) {
        perror("mkfifo failed");
        return -1;
    }

    pid_t pid = fork();
    
    if (pid == 0) {  // Child process
        // Open the FIFO for reading
        fd = open(fifo_path, O_RDONLY);
        if (fd == -1) {
            perror("Child failed to open FIFO");
            return -1;
        }

        // Read the data from FIFO
        read(fd, buffer, sizeof(buffer));
        cout << "Child received: " << buffer << endl;

        close(fd);
    } else {  // Parent process
        // Open the FIFO for writing
        fd = open(fifo_path, O_WRONLY);
        if (fd == -1) {
            perror("Parent failed to open FIFO");
            return -1;
        }

        // Write the message to FIFO
        write(fd, message, strlen(message) + 1);  // Including null terminator
        cout << "Parent sent: " << message << endl;

        close(fd);
    }

    return 0;
}


/* Compile:
   g++ NamedPipe_FIFO.cpp -o NamedPipe_FIFO
   Run:
   ./NamedPipe_FIFO
*/