#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MAX_SIZE 2048

int main(int argc, char *argv[])
{
    int fd;
    int remaining;
    int total_read = 0;
    int ret = 0, n = 0;
    int offset = 0;
    char buffer[MAX_SIZE];

    if (argc < 3 || argc > 4) {
        printf("Wrong usage\n");
        printf("Correct usage: ./devread <device_file> <readcount> [lseek_offset]\n");
        return 0;
    }

    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    remaining = atoi(argv[2]);
    printf("read requested = %d\n", remaining);

    // If lseek offset is provided
    if (argc == 4) {
        offset = atoi(argv[3]);
        ret = lseek(fd, offset, SEEK_CUR);
        if (ret < 0) {
            perror("lseek");
            close(fd);
            return -1;
        }
        printf("lseek to offset %d successful\n", offset);
    }

    printf("open was successful\n");

    while (n != 2 && remaining) {
        ret = read(fd, &buffer[total_read], remaining);
        if (ret == 0) {
            printf("end of file\n");
            break;
        } else if (ret > 0) {
            printf("read %d bytes of data\n", ret);
            total_read += ret;
            remaining -= ret;
        } else {
            perror("read");
            break;
        }
        n++;
    }

    printf("total_read = %d\n", total_read);

    for (int i = 0; i < total_read; i++)
        printf("%c", buffer[i]);
    printf("\n");

    close(fd);
    return 0;
}
