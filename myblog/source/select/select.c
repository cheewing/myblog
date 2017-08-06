/**
 * select.c
 * 多路复用select的用法
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define fifo_filename "test_fifo"

int main(int argc, char *argv[])
{
    fd_set rfds;
    struct timeval tv;
    int ret; 
    int fd;

    ret = mkfifo(fifo_filename, 0666);
    if (ret != 0)
        perror("mkfifo error");

    fd = open(fifo_filename, O_RDWR);
    if (fd < 0) {
        perror("open error");
        exit(-1);
    }

    ret = 0;

    while (1) {
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        FD_SET(fd, &rfds);

        tv.tv_sec = 1;
        tv.tv_usec = 0;

        ret = select(FD_SETSIZE, &rfds, NULL, NULL, NULL);
        
        if (ret == -1) {
            perror("select error");
        } else if (ret > 0) {
            char buf[100] = {0};
            if (FD_ISSET(0, &rfds)) {
                read(0, buf, sizeof(buf));
                printf("stdin buf = %s\n", buf);
            } else if (FD_ISSET(fd, &rfds)) {
                read(fd, buf, sizeof(buf));
                printf("fifo buf = %s\n", buf);
            }
        } else if (ret == 0) {
            printf("time out\n");
        }
    }
    
    exit(0);
}
