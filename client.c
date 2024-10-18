#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"  // Server's IP address (localhost)
#define SERVER_PORT 8080        // Server's port
#define BUFFER_SIZE 1024        // Buffer size for communication

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // Create the client socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);

    // Convert the server's IP address from text to binary
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Connection to server failed");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server.\n");

    // Communication loop
    while (1) {
        // Get input from the user
        printf("Enter message (type 'exit' to quit): ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Send the message to the server
        send(clientSocket, buffer, strlen(buffer), 0);

        // Exit if the user types 'exit'
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Disconnecting from server...\n");
            break;
        }

        // Receive the server's response
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived <= 0) {
            printf("Server disconnected or error receiving data.\n");
            break;
        }

        // Null-terminate the received data and print it
        buffer[bytesReceived] = '\0';
        printf("Server: %s", buffer);
    }

    // Close the socket
    close(clientSocket);
    return 0;
}
