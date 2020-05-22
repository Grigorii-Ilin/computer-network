#include <stdio.h> 
#include <windows.h>
#include <locale.h>
#include "server.h"


void clear_buf(char* b) {
    int i;
    for (i = 0; i < MAX_DATA_SIZE; i++)
        b[i] = '\0';
}


void send_ok(const int socket) {
    const char* msg = "OK\0";
    send(socket, msg, (int)strlen(msg), 0);
}


void show_stats(struct Statistics stats[], int statsLen) {
    printf("\n***********\n");
    for (int i = 0; i < statsLen; i++) {
        printf("client id: %d, file name: %s, file size: %d", stats[i].client_id, stats[i].file_name, stats[i].file_size);
    }
}



int main() {
    setlocale(LC_ALL, "RUS");
    
    //int const MAX_WORK = 5;
    struct Statistics statistics[5];
    int statIndex = 0;

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


    while (statIndex< 5) {  
        int iSocket2;
        struct sockaddr_in client;
        int sin_size;
        sin_size = sizeof(struct sockaddr_in);
        iSocket2 = accept(iSocket1, (struct sockaddr*) & client, &sin_size);

        printf("Порт клиента: %d", client.sin_port);
        statistics[statIndex].client_id = client.sin_port;

 
        int numbytes;

        //char fileName[MAX_DATA_SIZE];  
        numbytes = recv(iSocket2, statistics[statIndex].file_name, MAX_DATA_SIZE, 0);
        printf("Имя файла: %s\n", statistics[statIndex].file_name);
        send_ok(iSocket2);


        char fileLen[MAX_DATA_SIZE];
        numbytes = recv(iSocket2, fileLen, MAX_DATA_SIZE, 0);// like read in linux
        statistics[statIndex].file_size = atoi(fileLen);
        printf("Длина файла в байтах: %d\n", statistics[statIndex].file_size);
        send_ok(iSocket2);


        FILE* file = fopen(statistics[statIndex].file_name, "w+");
        char buffer[MAX_FILE_BUFFER_SIZE];
        while ((recv(iSocket2, buffer, MAX_FILE_BUFFER_SIZE, 0)) > 0) {
            fwrite(buffer, sizeof(char), strlen(buffer), file);
            clear_buf(buffer);
            send_ok(iSocket2);
        }
        printf("Файл %s сохранен\n", statistics[statIndex].file_name);
    
        fclose(file);
        closesocket(iSocket2);
        statIndex++;
        }

    WSACleanup();

    show_stats(statistics, statIndex);

    return 0;
}