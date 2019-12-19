#ifndef HEADER_FILE
#define HEADER_FILE
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>
#include <signal.h>

short create_socket();
int  bind_socket(int sock, int portnumber, char* ip_addr);
void list_directory(int* client_socket, char* root_dir);
int receive_filename_send(int client_socket);
void initialize_client(int client_socket, int portnumber, char* ip_addr);
void get_file_send(int* server_socket, int* client_socket);
void initialize_server(int server_socket, int portnumber, char* ip_addr, char * root_dir);
int receive_content(int client_socket);
void send_filename(int client_socket);
void receive_info(int client_socket);
void connecto_server(int client_socket, int portnumber, char* ip_addr);

#endif