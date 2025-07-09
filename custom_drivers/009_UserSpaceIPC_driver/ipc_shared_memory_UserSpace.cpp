#include <iostream>
#include <cstring>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

struct SharedData {
    int ready;           // Flag: 0 = empty, 1 = data ready
    char message[256];   // Message buffer
};

int main() {
    size_t size = sizeof(SharedData);
    SharedData* shared = (SharedData*) mmap(nullptr, size, 
        PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (shared == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    shared->ready = 0;

    pid_t pid = fork();

    if (pid == 0) {
        // 👦 Child process: CONSUMER
        std::cout << "[Consumer] Waiting for message..." << std::endl;

        while (shared->ready == 0) {
            usleep(10000); // Sleep 10ms
        }

        std::cout << "[Consumer] Received: " << shared->message << std::endl;

        shared->ready = 0; // Reset
        munmap(shared, size);
    } else {
        // 👨 Parent process: PRODUCER
        const char* msg = "Hello from producer!";
        std::cout << "[Producer] Writing: " << msg << std::endl;

        strcpy(shared->message, msg);
        shared->ready = 1;

        wait(nullptr); // Wait for child to finish
        munmap(shared, size);
    }

    return 0;
}


/* g++ ipc_shared_memory_UserSpace.cpp -o ipc_shared_memory_UserSpace */
/* ./ipc_shared_memory_UserSpace */