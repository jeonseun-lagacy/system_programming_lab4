#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 3490
#define BACKLOG 10
#define BUF_SIZE 512
#define SERVER_MSG "Welcome to server!!"
int main(){
    int ssock, csock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    char buf[BUF_SIZE];
    int sin_size;

    if((ssock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(ssock, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr)) == -1){
        perror("bind");
        exit(1);
    }

    if(listen(ssock, BACKLOG) == -1){
        perror("listen");
        exit(1);
    }
        sin_size = sizeof(struct sockaddr_in);
        if((csock = accept(ssock, (struct sockaddr*)&clnt_addr, &sin_size)) == -1){
            perror("accept");
            exit(1);
        }
        while(1) {
            printf("Server : got connection from %s\t", inet_ntoa(clnt_addr.sin_addr));
            memset(buf, 0, BUF_SIZE);

            if (recv(csock, buf, BUF_SIZE, 0) == -1) {
                perror("recv");
                exit(1);
            }
            printf("client : %s", buf);

            memset(buf, 0, BUF_SIZE);
            strcpy(buf, SERVER_MSG);
            printf("%s\n", buf);
            if (send(csock, buf, strlen(buf), 0) == -1) {
                perror("send");
                close(csock);
                exit(0);
            }

        }close(csock);
    close(ssock);
    return 0;
}