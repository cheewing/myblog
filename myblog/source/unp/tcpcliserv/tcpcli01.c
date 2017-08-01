#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <strings.h>
#include <errno.h>
#include <string.h>

#define SERV_PORT 9879
#define MAXLINE 1024

void str_cli(FILE *fp, int sockfd);
ssize_t writen(int fd, const void *ptr, size_t n);
ssize_t readline(int fd, void *vptr, size_t maxlen);

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 2) {
        perror("usage: tcpcli <IPaddress>");
        exit(-1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(-1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr));

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect error");
        exit(-1);
    }

    str_cli(stdin, sockfd);

    exit(0);
}

void str_cli(FILE *fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];
    
    while (fgets(sendline, MAXLINE, fp) != NULL) {
        if (writen(sockfd, sendline, strlen(sendline)) < 0) {
            perror("writen error");
            exit(-1);
        }

        if (readline(sockfd, recvline, MAXLINE) == 0) {
            perror("str_cli: server terminated prematurely");
            exit(-1);
        }

        if (fputs(recvline, stdout) == EOF) {
            perror("fputs error");
            exit(-1);
        }
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

ssize_t readline(int fd, void *vptr, size_t maxlen)
{
    ssize_t n, rc;
    char c, *ptr;

    ptr = vptr;
    for (n = 1; n < maxlen; n++) {
    again:
        if ((rc = read(fd, &c, 1)) == 1) {
            *ptr++ = c;
            if (c == '\n')
                break;
        } else if (rc == 0) {
            *ptr = 0;
            return (n - 1);
        } else {
            if (errno == EINTR)
                goto again;
            return (-1);
        }
    }
    *ptr = 0;
    return (n);
}
