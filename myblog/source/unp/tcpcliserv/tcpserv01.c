#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <errno.h>

#define SERV_PORT 9879
#define LISTENQ 5
#define MAXLINE 1024

ssize_t writen(int fd, const void *ptr, size_t n);
void str_echo(int sockfd);

int main(int argc, char *argv[])
{
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        perror("socket error");
        exit(-1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    int ret_bind = bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (ret_bind < 0) {
        perror("bind error");
        exit(-1);
    }

    if (listen(listenfd, LISTENQ) < 0) {
        perror("listen error");
        exit(-1);
    }

    for ( ; ; ) {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
        if (connfd < 0) {
            perror("accept error");
            exit(-1);
        }

        if ((childpid = fork()) < 0) {
            perror("fork error");
            exit(-1);
        } else if (childpid == 0) {
            if (close(listenfd) < 0) {
                perror("close error");
                exit(-1);
            }
            
            str_echo(connfd);
            exit(0);
        }

        if (close(connfd) < 0) {
            perror("close error");
            exit(-1);
        }
    }


    exit(0);
}

void str_echo(int sockfd)
{
    ssize_t n;
    char buf[MAXLINE];

again:
    while ((n = read(sockfd, buf, MAXLINE)) > 0) {
        if (writen(sockfd, buf, n) < 0) {
            perror("writen error");
            exit(-1);
        }
    }    

    if (n < 0 && errno == EINTR)
        goto again;
    else if (n < 0) {
        perror("str_echo: read error");
        exit(-1);
    }
}

ssize_t writen(int fd, const void *ptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;

    nleft = n;
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) < 0) {
            if (nleft == n)
                return (-1);
            else 
                break;
        } else if (nwritten == 0) {
            break;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return (n - nleft);
}
