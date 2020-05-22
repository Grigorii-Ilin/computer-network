#pragma once

#define PORT 3550 
#define MAX_DATA_SIZE 250 
#define MAX_FILE_BUFFER_SIZE 1024

int f_size(const FILE* f);
void get_ok_from_server(const int socket);
void send_file_name(const int socket, const char* fileName);
void send_file_size(const int socket, const FILE* f);
void send_file(const int socket, const FILE* f);