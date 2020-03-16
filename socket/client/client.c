#include <windows.h> 
#include <stdio.h> 
#include <locale.h>
#include "vigenere_lib.h"

#define PORT 3550 
#define MAXDATASIZE 250 

int main(int argc, char* argv[]){
	setlocale(LC_ALL, "RUS");

	printf("%s", "¬ведите пароль (A-Z):\n");
	char key[255];
	scanf_s("%254s", key, 255);

	WSADATA wsdata;
	//https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup
	WSAStartup(MAKEWORD(2, 2), &wsdata);

	struct hostent* he = gethostbyname("localhost");
	int iConnectedSocket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr = *((struct in_addr*)he->h_addr);

	connect(iConnectedSocket, (struct sockaddr*) & server, sizeof(struct sockaddr));

	char buf[MAXDATASIZE];
	int numbytes = recv(iConnectedSocket, buf, MAXDATASIZE, 0);
	buf[numbytes] = '\0';
	printf("—ообщение от сервера: %s\n", buf);

	decrypt(key, strlen(key), buf, numbytes);

	send(iConnectedSocket, buf, (int)strlen(buf), 0);

	closesocket(iConnectedSocket);
	WSACleanup();

	return 0;
}