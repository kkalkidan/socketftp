#include "header.h"
#define OK 0
#define STOP 1
#define RESTART -1


// creating a socket for the server or client 
short create_socket() {
    short sock;

    printf("\nCreating socket .. \n");

    sock = socket(AF_INET, SOCK_STREAM, 0);

    return sock;
}

//  binding a socket 
int  bind_socket(int sock, int portnumber, char* ip_addr){
    int opt = 1;
    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(portnumber);
    // server address 
    server_address.sin_addr.s_addr = inet_addr(ip_addr);

    char *s = inet_ntoa(server_address.sin_addr);
    printf("Server address %s", s);

    // Forcefully attaching socket to the port 8080 
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    }

    int bind_val = bind(sock, (struct sockaddr*) &server_address, sizeof(server_address));
    return bind_val;
}

int receive_filename_send(int client_socket) {
    char client_response[30];
    int recv_val; 
    printf("\n Receive file name ............ \n");
    recv_val = recv(client_socket, client_response, sizeof(client_response), 0);
    if(recv_val < 0){
        printf("Receive failed");
        return RESTART;
    }
    if(strcmp(client_response, "q") == 0) {
        printf("\nClient wishes to close communication channel \n");
        send(client_socket, "", 2, 0);
        return STOP;
    }
    

    FILE *fptr;

    fptr = fopen(client_response, "r");

    char buffer[255];

    printf("\nReceived download request for  %s from the client\n", client_response);

    if(fptr == NULL){
        printf("Error opening file"); 
    }else {
        while(fgets(buffer, 255, (FILE *)fptr)){
            // printf("Retrived buffer:  %s\n", buffer);
            send(client_socket, buffer, sizeof(buffer), 0);
        }
        fclose(fptr);
        printf("\n Finished sending the file \n\n");
    }
    send(client_socket, "", 1, 0);
    return RESTART;

}

void list_directory(int* client_socket, char* root_dir) {

    DIR *dir;
    struct dirent *file;

    if(!(dir = opendir(root_dir)))
        return ;
    int i = 0, send_val;
    int first = 1;
    while((file = readdir(dir)) != NULL){
        
        if(file->d_type != DT_DIR)  {

            if(first == 1)
            {
                char list_file[256] = "\n List of files \n";
                send(*client_socket, list_file, sizeof(list_file), 0);
                first = 0;
            }
            send_val = send(*client_socket, file->d_name, sizeof(file->d_name), 0);
            // printf("%d\n", send_val);
        }
       
}
    send(*client_socket, "", 1, 0);
}

void initialize_server(int server_socket, int portnumber, char* ip_addr, char * root_dir){

   
    
    if(portnumber < 1024 || portnumber > 65535 ){
        printf("\n invalid port number, valid range: 49152-65535\n");
        exit(1);
    }
    int bind_val;
    
    //creating socket 
    // server_socket = create_socket();
    if(server_socket < 0){
        printf("error in creating a socket");
        exit(1);
    }
    // bind socket and address
    bind_val = bind_socket(server_socket, portnumber, ip_addr);
    // bind_socket(server_sock, int portnumber, char* ip_addr)

    if(bind_val < 0){
        printf("Error in binding \n");
        exit(1);
    }

    listen(server_socket, 2);
       
    int client_socket;
    struct sockaddr_in client_address;
    int client_len;

    int status =0;

    if(client_socket < 0) {
        printf("Error in accepting client connection");
        exit(1);
            }

    while(1) {

        client_len = sizeof(struct sockaddr_in);
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, (socklen_t *) &client_len);

        if(client_socket < 0) {
            printf("Error in accepting client connection");
            exit(1);
        }
        char *s = inet_ntoa(client_address.sin_addr);
        printf("\nClient with IP address: %s is connected. \n", s);

        int child1 = fork();
        
        // printf("%d", child1);
        if(child1 == 0){

            list_directory(&client_socket, root_dir);
            pid_t childPID = getpid();
            pid_t parentPID = getppid();
            printf("current %d parent %d", childPID, parentPID);
            while(receive_filename_send(client_socket) == RESTART);
            printf("\n***********Channel closed ********* \n");
            kill(childPID, SIGKILL);
            exit(0);
            
        }
        if(child1 == -1) {
            printf("fork returned -1\n");
            break;
        }
        printf("ended child");
    }
    printf("parent process waiting for child process");
    while(wait(&status) > 0);
    close(server_socket);

}

void connecto_server(int client_socket, int portnumber, char* ip_addr){
    struct sockaddr_in server_address, client_address;

    server_address.sin_family = AF_INET;
    // htons: convert port number to network address format 
    server_address.sin_port = htons(portnumber);
    // server address 
    server_address.sin_addr.s_addr =  inet_addr(ip_addr);

    char *s = inet_ntoa(server_address.sin_addr);
    printf("\nserver address %s\n", s);
    
    int connect_status = connect(client_socket, (struct sockaddr *) &server_address, sizeof(struct sockaddr_in));

    // connect to another socket using connect function
    if(connect_status < 0){
        printf("connect failed");
    }
}

void receive_info(int client_socket) {
    while(1){
         char server_response[256];
         int recv_val = recv(client_socket, &server_response, sizeof(server_response), 0);

        if(recv_val > 0 ){
            printf("\t %s \n", server_response);
        }
        if(recv_val == 1){
            break;
        }
    }
}

void send_filename(int client_socket) {
    printf("Sending to server");
    int shortRetval = -1;
    struct timeval tv;
    tv.tv_sec = 20;  /* 20 Secs Timeout */
    tv.tv_usec = 0;
    if(setsockopt(client_socket,SOL_SOCKET,SO_SNDTIMEO,(char *)&tv,sizeof(tv)) < 0)
    {
        printf("Time Out\n");
        return ;
    }
    
    char file_name[30];
    printf("\n Please enter the name of the file you want to download \n");
    printf(" Enter q to exit:\n");
    scanf("%s", file_name);
    int length = (int) strlen(file_name);
    if(strcmp(file_name, "q") == 0){
        printf("Good bye!\n");
        send(client_socket, file_name, sizeof(file_name), 0);
        return;

    }
    printf("\nRequesting %s from the server .... \n", file_name);
    send(client_socket, file_name, sizeof(file_name), 0);
}

int receive_content(int client_socket) {

    char file_content[255];
    
    while(1) {
        int recv_val = recv(client_socket, &file_content, sizeof(file_content), 0);
        if(recv_val > 0){
            printf("%s", file_content);
        }if(recv_val == 1){
            return RESTART;
        }if(recv_val == 2){
            return STOP;
        }
    }
    return OK;

}
void initialize_client(int client_socket, int portnumber, char* ip_addr){
   
    //creating socket 
    // client_socket = create_socket();
    if(portnumber < 1024 || portnumber > 65535 ){
        printf("\n invalid port number, valid range: 49152-65535\n");
        exit(1);
    }
    connecto_server(client_socket, portnumber, ip_addr);
    receive_info(client_socket);
    while(1){
       send_filename(client_socket);
       if(receive_content(client_socket) == STOP){
           break;
       }
    }
}

