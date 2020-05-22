#define _CRT_SECURE_NO_WARNINGS

#include <windows.h> 
#include <stdio.h> 
#include <locale.h>
#include "client.h"


int f_size(const FILE* f) {
	int prev = ftell(f);
	fseek(f, 0L, SEEK_END);
	int sz = ftell(f);
	fseek(f, prev, SEEK_SET); //go back to where we were
	return sz;
}


void get_ok_from_server(const int socket) {
	char tmp[MAX_DATA_SIZE];
	recv(socket, tmp, MAX_DATA_SIZE, 0);
}


void send_file_name(const int socket, const char* fileName) {
	send(socket, fileName, strlen(fileName) * sizeof(char) + sizeof(char), 0);
	get_ok_from_server(socket);
}


void send_file_size(const int socket, const FILE* f) {
	char digits[17];
	_itoa(f_size(f), digits, 10);
	send(socket, digits, strlen(digits) * sizeof(char) + sizeof(char), 0);
	get_ok_from_server(socket);
}


void send_file(const int socket, const FILE* f) {
	char buffer[MAX_FILE_BUFFER_SIZE];

	while (!feof(f)) {
		int bytesReaded = fread(buffer, sizeof(char), MAX_FILE_BUFFER_SIZE, f);
		send(socket, buffer, bytesReaded, 0);
		get_ok_from_server(socket);
	}
}


int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "RUS");

	FILE* inputedFile = fopen(argv[1], "r+");

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

	send_file_name(iConnectedSocket, argv[1]);
	send_file_size(iConnectedSocket, inputedFile);
	send_file(iConnectedSocket, inputedFile);

	closesocket(iConnectedSocket);
	WSACleanup();

	fclose(inputedFile);

	return 0;
}