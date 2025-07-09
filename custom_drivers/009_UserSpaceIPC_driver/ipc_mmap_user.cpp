#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>

#define SHARED_MEM_SIZE 4096

int main() {
    int fd = open("/dev/ipc_mmap", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    char* addr = (char*) mmap(nullptr, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    // Write something
    strcpy(addr, "Hello from mmap user program!");

    // Read it back
    std::cout << "User-space mmap read: " << addr << std::endl;

    munmap(addr, SHARED_MEM_SIZE);
    close(fd);
    return 0;
}

/* g++ ipc_mmap_user.cpp -o ipc_mmap_user */
/* ./ipc_mmap_user */
