// receiver.c
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/wait.h>
#include<unistd.h>
#include<fcntl.h>
#include<semaphore.h>

#define SHM_NAME "/my_autosar_shm"
#define SHM_SIZE 4096
#define SEM_NAME "/my_autosar_sem"

int main() {
    /*1. Open the shared memory*/
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if(shm_fd == -1)
    {
        perror("SHM open failed\n");
        return 1;
    }
    /*2. Truncate the meory to 4Kb*/
    ftruncate(shm_fd, SHM_SIZE);
    void *ptr = mmap(NULL, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(ptr == MAP_FAILED)
    {
        perror("MMAP failed\n");
        return 1;
    }
    /*3. Open Semaphore*/
    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 0);
    if(sem == SEM_FAILED)
    {
        perror("sem_open failed\n");
        return 1;
    }
    /*4. wait for sender to post*/
    printf("Receiver: waiting for sender...\n");
    sem_wait(sem); 

    printf("Receiver: got message from shared memory: %s\n", (char *)ptr);

    /*5. Unmap the memory*/
    munmap(ptr, SHM_SIZE);
    close(shm_fd);
    sem_close(sem);

    // Optional: Clean up resources
    shm_unlink(SHM_NAME);
    sem_unlink(SEM_NAME);

    return 0;
}

/* Compile:
   g++ ipc_SharedMemory_Receiver.c -o ipc_SharedMemory_Receiver
   Run:
   ./ipc_SharedMemory_Receiver
*/