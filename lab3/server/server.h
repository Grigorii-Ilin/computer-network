#pragma once

#define PORT 3550
#define MAX_DATA_SIZE 250 
#define MAX_FILE_BUFFER_SIZE 1024

struct Statistics {
	int	client_id;
	char file_name[MAX_DATA_SIZE];
	int file_size;
};


void clear_buf(char* b);
void send_ok(const int socket);
void show_stats(struct Statistics stats[], int statsLen);