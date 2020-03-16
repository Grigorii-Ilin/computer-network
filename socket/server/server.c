#include <stdio.h> 
#include <windows.h>

#define PORT 3550
//#define BACKLOG 2
#define MAXDATASIZE 250 

int main(int argc, char** argv) {
    WSADATA wsdata; 
    //WSAStartup(0x0101,&wsdata); 
    //https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup
    WSAStartup(MAKEWORD(2, 2), &wsdata);
    int fd, fd2;
    struct sockaddr_in server;
    struct sockaddr_in client; 
    int sin_size; 

    if ((fd=socket(AF_INET, SOCK_STREAM, 0)) == -1 ) { 
        printf("socket() error\n"); 
        exit(-1); 
    } 

    server.sin_family = AF_INET; 
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY; 

    if (bind(fd,(struct sockaddr*)&server, sizeof(struct sockaddr))==-1) { 
        printf ("bind() error\n"); 
        exit (-1); 
    } 

    if (listen (fd,2) == -1) { 
        printf ("listen() error\n"); 
        exit (-1); 
    } 

    boolean clientConnected = FALSE;
    do{
        sin_size=sizeof (struct sockaddr_in); 
        if ((fd2 = accept(fd,(struct sockaddr*)&client,&sin_size)) == -1){ 
            printf ("accept() error\n"); 
            exit (-1); 
        } 
        printf ("You got a connection from %s\n",inet_ntoa(client.sin_addr) ); 
        send (fd2,"111111\n", (int)strlen("111111\n"),0);

        clientConnected = TRUE;

    } while (clientConnected==FALSE);


    int numbytes;
    char buf[MAXDATASIZE];
    if ((numbytes = recv(fd2, buf, MAXDATASIZE, 0)) == -1) // like read in linux
    {
        printf("recv() error\n");
        exit(-1);
    }
    buf[numbytes] = '\0';
    printf("Client Message: %s\n", buf);


    closesocket (fd2); 
    WSACleanup();

    return 0;
}