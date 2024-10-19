#ifndef SERVER_TOOLS
#define SERVER_TOOLS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>        // For close()
#include <arpa/inet.h>     // For inet_addr(), htons(), sockaddr_in
#include <sys/socket.h>    // For socket(), bind(), listen()


// To be defined in .c file
void handleClientQuery(int clientSocket);

int initializeServer(int port, int backlog, int *server_fd) {

    printf("Creating TCP socket\n");
    *server_fd = socket(AF_INET, SOCK_STREAM, 0);  // TCP socket
    if (*server_fd < 0) {
        perror("Socket creation failed");
        return -1;
    }

    printf("Created TCP socket\n");

    //Reuse addr sock opt
    int opt = 1;
    if (setsockopt(*server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Set socket options failed");
        close(*server_fd);
        return -1;
    }

    printf("Set sock opts\n");

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));  
    server_addr.sin_family = AF_INET;             
    server_addr.sin_addr.s_addr = INADDR_ANY;      
    server_addr.sin_port = htons(port);           
    
    if (bind(*server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(*server_fd);
        return -1;
    }

    printf("Finished bind\n");


    if (listen(*server_fd, backlog) < 0) {
        perror("Listen failed");
        close(*server_fd);
        return -1;
    }

    printf("Server is listening on port %d...\n", port);
    return 0; 
}

void acceptConnectionAtFork(int serverSocket){
    int clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrlen = sizeof(clientAddr);

    while (1) {
        if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrlen)) == -1) {
            perror("Accept failed");
            continue; 
        }

        printf("New client connected.\n");

        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            close(clientSocket);
            continue;
        }

        if (pid == 0) {
            close(serverSocket);  
            handleClientQuery(clientSocket);
        } else {
            close(clientSocket);
        }
    }
}



#endif