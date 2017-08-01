#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <poll.h>

#define fifo_filename "test_fifo"

int main(int argc, char *argv[])
{
    int ret;
    int fd;
    struct pollfd fds[2];

    ret = mkfifo(fifo_filename, 0666);
    if (ret != 0) {
        perror("mkfifo error");
    }

    fd = open(fifo_filename, O_RDWR);
    if (fd < 0) {
        perror("open error");
        exit(-1);
    }

    ret = 0;
    
    fds[0].fd = 0;
    fds[1].fd = fd;

    fds[0].events = POLLIN;
    fds[1].events = POLLIN;

    while (1) {
        ret = poll(fds, 2, -1);

        if (ret == -1) {
            perror("epoll_wait error");
        } else if (ret > 0) {
            char buf[100] = {0};

            if ((fds[0].revents & POLLIN) == POLLIN) {
                read(0, buf, sizeof(buf));
                printf("stdin buf = %s\n", buf);
            } else if ((fds[1].revents & POLLIN == POLLIN)) {
                read(fd, buf, sizeof(buf));
                printf("fifo buf = %s\n", buf);
            }
        } else if (ret == 0) {
            printf("time out\n");
        }
    }

    exit(0);
}
