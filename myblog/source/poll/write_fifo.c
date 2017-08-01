#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define fifo_filename "test_fifo"

int main(int argc, char *argv[])
{
    int ret = 0;
    int fd;

    ret = mkfifo(fifo_filename, 0666);
    if (ret != 0) {
        perror("mkfifo error");
    }

    fd = open(fifo_filename, O_RDWR);
    if (fd < 0) {
        perror("open error");
        exit(-1);
    }

    while (1) {
        char *str = "this is for test";
        write(fd, str, strlen(str));
        printf("after write to fifo\n");
        sleep(5);
    }

    exit(0);
}
