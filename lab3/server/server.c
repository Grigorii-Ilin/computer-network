#include <stdio.h> 
#include <windows.h>
#include <locale.h>



#define PORT 3550
#define MAX_DATA_SIZE 250 
#define MAX_FILE_BUFFER_SIZE 1024


void clearBuf(char* b) {
    int i;
    for (i = 0; i < MAX_DATA_SIZE; i++)
        b[i] = '\0';
}


void sendOK(int sock) {
    send(sock, "OK\0", (int)strlen("OK\0")+1, 0);
}


int main() {
    setlocale(LC_ALL, "RUS");

    //printf("%s", "Введите пароль (A-Z):\n");
    //char key[255];
    //scanf_s("%254s", key, 255);

    //printf("%s", "Введите текст для шифрования (A-Z):\n");
    //char txt[255];
    //scanf_s("%254s", txt, 255);

    //encrypt(key, strlen(key), txt, strlen(txt));

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
    //send(iSocket2, txt, (int)strlen(txt), 0);
 
    int numbytes;

    char fileName[MAX_DATA_SIZE];  
    numbytes = recv(iSocket2, fileName, MAX_DATA_SIZE, 0);
    //fileName[numbytes+1] = '\0';
    printf("Имя файла: %s\n", fileName);
    sendOK(iSocket2);

    
    //clearBuf(buf);
    char fileLen[MAX_DATA_SIZE];
    numbytes = recv(iSocket2, fileLen, MAX_DATA_SIZE, 0);// like read in linux
    //fileLen[numbytes] = '\0';
    printf("Длина файла в байтах: %s\n", fileLen);
    sendOK(iSocket2);


    //************************int fileLenInt = atoi(fileLen);
    FILE* file = fopen(fileName, "w+");
    char buffer[MAX_FILE_BUFFER_SIZE];
    while ((recv(iSocket2, buffer, MAX_FILE_BUFFER_SIZE, 0)) > 0) {
        //printf("%s", buffer);       
        sendOK(iSocket2);
        fwrite(buffer, sizeof(char), strlen(buffer), file);
        clearBuf(buffer);
    }
    printf("Файл %s сохранен\n", fileName);
    fclose(file);


    closesocket(iSocket2);
    WSACleanup();

    return 0;
}