#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/epoll.h>

#define fifo_filename "test_fifo"

int main(int argc, char *argv[])
{
    int ret;
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

    ret = 0;
    struct epoll_event event;
    struct epoll_event wait_event;

    int epfd = epoll_create(10);
    if (epfd == -1) {
        perror("epoll_create error");
        exit(-1);
    }

    event.data.fd = 0;
    event.events = EPOLLIN;

    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, 0, &event);
    if (ret == -1) {
        perror("epoll_ctl error");
        exit(-1);
    }

    event.data.fd = fd;
    event.events = EPOLLIN;

    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    if (ret == -1) {
        perror("epoll_ctl error");
        exit(-1);
    }

    ret = 0;

    while (1) {
        ret = epoll_wait(epfd, &wait_event, 2, -1);

        if (ret == -1) {
            close(epfd);
            perror("epoll_wait error");
        } else if (ret > 0) {
            char buf[100] = {0};

            if ((wait_event.data.fd == 0) && (wait_event.events & EPOLLIN == EPOLLIN)) {
                read(0, buf, sizeof(buf));
                printf("stdin buf = %s\n", buf);
            } else if ((wait_event.data.fd == fd) && (wait_event.events & EPOLLIN == EPOLLIN)) {
                read(fd, buf, sizeof(buf));
                printf("fifo buf = %s\n", buf);
            }
        } else if (ret == 0) {
            printf("time out\n");
        }
    }

    exit(0);
}
