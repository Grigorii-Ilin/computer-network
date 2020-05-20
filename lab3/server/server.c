#include <stdio.h> 
#include <windows.h>
#include <locale.h>



#define PORT 3550
#define MAXDATASIZE 250 

int main() {
    setlocale(LC_ALL, "RUS");

    printf("%s", "Введите пароль (A-Z):\n");
    char key[255];
    scanf_s("%254s", key, 255);

    printf("%s", "Введите текст для шифрования (A-Z):\n");
    char txt[255];
    scanf_s("%254s", txt, 255);

    encrypt(key, strlen(key), txt, strlen(txt));

    WSADATA wsdata; 
    //https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup
    WSAStartup(MAKEWORD(2, 2), &wsdata);

    int iSocket1;
    iSocket1 = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server;
    server.sin_family = AF_INET; 
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY; 

    bind(iSocket1, (struct sockaddr*) & server, sizeof(struct sockaddr));

    printf("%s", "Ожидаем клиента...\n");
    listen(iSocket1, 1);

    int iSocket2;
    struct sockaddr_in client;
    int sin_size;
    sin_size = sizeof(struct sockaddr_in);
    iSocket2 = accept(iSocket1, (struct sockaddr*) & client, &sin_size);
    send(iSocket2, txt, (int)strlen(txt), 0);
 
    int numbytes;
    char buf[MAXDATASIZE];
    numbytes = recv(iSocket2, buf, MAXDATASIZE, 0);// like read in linux
    buf[numbytes] = '\0';
    printf("Сообщение от клиента: %s\n", buf);

    closesocket(iSocket2);
    WSACleanup();

    return 0;
}