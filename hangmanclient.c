#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#define BUF_SIZE 50

void error_handling(char *message);

int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    pthread_t send_thread, recv_thread;
    
    if(argc!=3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    
    //socket
    sock=socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_port=htons(atoi(argv[2]));
    
    //connect
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
        error_handling("connect() error!");
    puts("Connecting success!!!");
    
    
    /write&read
    pthread_create(&wrt_thread, NULL, wrt, (void *)&sock);
    pthread_create(&read_thread, NULL, read, (void *)&sock);
    
    pthread_join(wrt_thread, &thread_return);
    pthread_join(read_thread, &thread_return);
    
    //close
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void wrt(void * arg) {
    
    int sock = *((int *)arg);
    char input[BUF_SIZ];
    while(strcmp("quit", input )== 0) {
        fgets(input, BUF_SIZ, stdin);
        write(sock, input, BUF_SIZE);
    }
}

void read(void * arg) {
    
    int sock = *((int *)arg);
    char input[BUF_SIZ];
    while(1) {
        read(sock, &message, BUF_SIZE-1);
        printf("%s", message);
    }
}

