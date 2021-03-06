#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAXLEN 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sock;
    int clnt_sock;
    int read_len , idx;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    
    char message[MAXLEN]="Hello World!";
    
    int usercount = 0;
    int user[2] = {0}; //user 소켓
    
    //socket
    serv_sock=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //양방향통신
    if(serv_sock == -1)
        error_handling("socket() error");
    
    if(argc!=2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));
    
    //bind
    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
        error_handling("bind() error");
    
    //listen
    if(listen(serv_sock, 5)==-1)
        error_handling("listen() error");
    
    clnt_addr_size=sizeof(clnt_addr);
    
    //read
    while(1)
    {
        puts("Server] Listening...");
        clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
        if(clnt_sock==-1)
            error_handling("accept() error");
        puts("Server] Accepted!!");
        
        usercount++;
        
        if(usercount<=2){ // player 2명 받기
            user[usercount-1]=clnt_sock;
            
            send(clnt_sock,"Welcome to HangmanGame!\n",strlen("Welcome to HangmanGame!\n"),0);
            
            if(usercount==1){ //player 1
                send(clnt_sock,"you are player1\n",strlen("you are player1\n"),0);
                send(clnt_sock,"상대방이 접속하면 게임이 시작됩니다.\n",strlen("상대방이 접속하면 게임이 시작됩니다.\n"),0);
            }
            else if(usercount==2){ //player 2
                send(clnt_sock,"you are player2\n",strlen("you are player2\n"),0);
                send(clnt_sock,"잠시 후 게임이 시작됩니다.\n",strlen("잠시 후 게임이 시작됩니다.\n"),0);
            }
        }
        else // player 2명 다 차면
            send(clnt_sock,"정원이 다 찼습니다.\n",strlen("정원이 다 찼습니다.\n"),0);
        
        //게임시작
        send(user[0],"Game Start!\n",strlen("Game Start!\n"),0);
        send(user[1],"Game Start!\n",strlen("Game Start!\n"),0);
        
        
        message[0] = 0;
        idx = 0;
        while(read_len==read(clnt_sock, &message[idx], 1))
        {
            if(read_len==-1){
                error_handling("read() error!");
                exit(1);
            }
            
            if(message[idx++] == '\n'){
                message[idx] = 0;
                
                //-----------------------write code to do---------------------------------
                printf("Message from Client: %s", message);
                //------------------------------------------------------------------------
                message[0] = 0 ;
                idx = 0;
            }
        }
    }
    
    //close
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    perror(message);
    fputc('\n', stderr);
    exit(1);
}
