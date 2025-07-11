#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <cstdlib>

#define BUFFER_SIZE 5
#define PRODUCE_COUNT 10

using namespace std;

// Shared structure in mmap
struct SharedData {
    int buffer[BUFFER_SIZE];
    int in = 0;
    int out = 0;
    int count = 0;
    pthread_mutex_t lock;
};

// Producer thread
void* producer(void* arg) {
    SharedData* data = static_cast<SharedData*>(arg);
    for (int i = 0; i < PRODUCE_COUNT; ++i) {
        pthread_mutex_lock(&data->lock);
        if (data->count < BUFFER_SIZE) {
            data->buffer[data->in] = i + 1;
            cout << "Produced: " << data->buffer[data->in] << endl;
            data->in = (data->in + 1) % BUFFER_SIZE;
            data->count++;
        }
        pthread_mutex_unlock(&data->lock);
        usleep(100000); // simulate work
    }
    return nullptr;
}

// Consumer thread
void* consumer(void* arg) {
    SharedData* data = static_cast<SharedData*>(arg);
    for (int i = 0; i < PRODUCE_COUNT; ++i) {
        pthread_mutex_lock(&data->lock);
        if (data->count > 0) {
            int item = data->buffer[data->out];
            cout << "Consumed: " << item << endl;
            data->out = (data->out + 1) % BUFFER_SIZE;
            data->count--;
        }
        pthread_mutex_unlock(&data->lock);
        usleep(150000); // simulate work
    }
    return nullptr;
}

int main() {
    // Allocate shared memory using mmap
    SharedData* shared = static_cast<SharedData*>(
        mmap(nullptr, sizeof(SharedData),
             PROT_READ | PROT_WRITE,
             MAP_SHARED | MAP_ANONYMOUS,
             -1, 0));

    if (shared == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    // Initialize mutex for shared memory (must use PTHREAD_PROCESS_SHARED)
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shared->lock, &mutex_attr);

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    pthread_t prod, cons;

    if (pid == 0) {
        // Child process
        cout << "\n[Child] Starting producer and consumer threads\n";
        pthread_create(&prod, nullptr, producer, shared);
        pthread_create(&cons, nullptr, consumer, shared);

        pthread_join(prod, nullptr);
        pthread_join(cons, nullptr);

        cout << "[Child] Threads finished, exiting\n";
        exit(0);  // return status to parent
    } else {
        // Parent process
        wait(nullptr);
        cout << "\n[Parent] Child has exited, cleaning up\n";

        pthread_mutex_destroy(&shared->lock);
        pthread_mutexattr_destroy(&mutex_attr);
        munmap(shared, sizeof(SharedData));
    }

    return 0;
}
