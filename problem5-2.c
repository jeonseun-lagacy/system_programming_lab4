#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 3490
#define MAX_DATA_SIZE 100

int main(int argc, char *argv[]){
    int csock, numbytes;
    char buf[MAX_DATA_SIZE];

    struct sockaddr_in serv_addr;
    int len;

    if(argc != 2){
        fprintf(stderr, "Usage: problem5-2 <server ip>");
        exit(1);
    }

    if((csock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(1);
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(PORT);

    if (connect(csock, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) == -1) {
        perror("connect");
        exit(1);
    }
    while(1) {
        memset(buf, 0, MAX_DATA_SIZE);
        printf("input message : ");
        fgets(buf, MAX_DATA_SIZE, stdin);
        len = strlen(buf);
        if (send(csock, buf, len, 0) != len) {
            fprintf(stderr, "send failed\n");
            exit(1);
        }
        memset(buf, 0, MAX_DATA_SIZE);
        if ((numbytes = recv(csock, buf, MAX_DATA_SIZE, 0)) == -1) {
            perror("recv");
            exit(1);
        }
        printf("Received message : %s\n", buf);
    }
    close(csock);
    return 0;
}