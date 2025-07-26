#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>

#define SHARED_MEM_SIZE 4096

int main() {
    // Open the device file
    int fd = open("/dev/ipc_mmap", O_RDWR);
    if (fd == -1) {
        std::cerr << "Error opening the device file" << std::endl;
        return -1;
    }

    // Map shared memory to the process's address space
    char *mapped_mem = (char*) mmap(NULL, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped_mem == MAP_FAILED) {
        std::cerr << "Error mapping memory" << std::endl;
        close(fd);
        return -1;
    }

    std::cout << "Memory mapped successfully at address: " << static_cast<void*>(mapped_mem) << std::endl;

    // Write data to the shared memory
    const char *message = "Hello from user space!";
    strncpy(mapped_mem, message, SHARED_MEM_SIZE);

    // Read data from the shared memory
    std::cout << "Data read from shared memory: " << mapped_mem << std::endl;

    // Clean up
    munmap(mapped_mem, SHARED_MEM_SIZE);
    close(fd);

    return 0;
}

/*
    Compile:
    g++ ipc_mmap_user.c -o ipc_mmap_user
    Run:
    ./ipc_mmap_user
*/
