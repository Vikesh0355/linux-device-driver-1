#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

#define SHARED_MEM_SIZE 4096

int main() {
    int fd = open("/dev/ipc_mmap", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Read current content
    char buffer[SHARED_MEM_SIZE] = {0};
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead < 0) {
        perror("read");
    } else {
        std::cout << "[User] Read from kernel: " << buffer << std::endl;
    }

    // Get user input to write
    std::cout << "[User] Enter new message to write: ";
    std::string input;
    std::getline(std::cin, input);

    if (write(fd, input.c_str(), input.length()) < 0) {
        perror("write");
    } else {
        std::cout << "[User] Wrote to kernel.\n";
    }

    close(fd);
    return 0;
}

/* Compile:
   g++ ipc_mmap_user.cpp -o ipc_rw_user
   Run:
   ./ipc_rw_user
*/
