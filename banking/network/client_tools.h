#ifndef CLIENT_TOOLS
#define CLIENT_TOOLS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>        
#include <arpa/inet.h>    
#include <sys/socket.h>   


int initializeClient(const char *server_ip, int port, int *client_socket) {

    *client_socket = socket(AF_INET, SOCK_STREAM, 0); 
    if (*client_socket < 0) {
        perror("Socket creation failed");
        return -1;
    }


    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));  
    server_addr.sin_family = AF_INET;            
    server_addr.sin_port = htons(port);       


    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        close(*client_socket);
        return -1;
    }

    // Step 4: Connect to the server
    if (connect(*client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(*client_socket);
        return -1;
    }

    printf("Connected to the server at %s:%d\n", server_ip, port);
    return 0;  // Success
}

int sendDisconnectPkt(int sockfd){
    send(sockfd, "", 0, 0);
}

#endif