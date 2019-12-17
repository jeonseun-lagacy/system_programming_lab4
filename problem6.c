#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 3490
#define MAX_BUF 256
#define QLEN 10
void *thread_run(void *arg){

}

int main(){
    struct sockaddr_in server_addr, client_addr; //
    int sock_fd, new_fd;//sock_fd : lisnter, new_fd : connector
    int alen; //address length

    fd_set readfds, activefds;

    int i, maxfd = 0, numbytes;
    char buf[MAX_BUF];

    if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket() failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if((bind(sock_fd, (struct sockaddr*) &server_addr, sizeof(server_addr))) < 0){
        perror("bind() failed");
        exit(1);
    }
    if (listen(sock_fd, QLEN) < 0) {
        fprintf(stderr,"listen failed\n");
        exit(1);
    }
    alen = sizeof(client_addr);
/* Initialize the set of active sockets. */
    FD_ZERO (&activefds);
    FD_SET (sock_fd, &activefds);
    maxfd = sock_fd;

    fprintf(stderr, "Server up and running.\n");
    while(1){
        printf("SERVER: Waiting for contact..., %d\n", maxfd);
        readfds = activefds;
        if (select (maxfd + 1, &readfds, NULL, NULL,
                    NULL) < 0) {
            perror ("select");
            exit (EXIT_FAILURE);
        }
        for (i = 0; i <= maxfd; ++i) {
            if (FD_ISSET (i, &readfds)) {
                if (i == sock_fd) {
                    if ( (new_fd=accept(sock_fd, (struct sockaddr *)&client_addr, &alen)) < 0) {
                        fprintf(stderr, "accept failed\n");
                        exit (1);
                    }
                    FD_SET(new_fd, &activefds);
                    printf("set\n");
                    if (new_fd > maxfd)
                        maxfd = new_fd;
                }
                else {
                    if ((numbytes=recv (i, buf, 100, 0)) ==
                        -1)
                        perror("recv");
                    buf[numbytes]='\0';
                    if (send (i, buf, strlen(buf), 0) == -1)
                        perror("send");
                    close(i);
                    FD_CLR(i, &activefds);
                }
            }
        }
    }
    close(sock_fd);
    return 0;
}