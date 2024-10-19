#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to handle SIGCHLD to prevent zombie processes
void handle_sigchld(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);  // Clean up all dead child processes
}

// Function to handle client communication
void handleClient(int clientSocket) {
    char buffer[BUFFER_SIZE];
    int bytesRead;

    printf("Handling new client...\n");

    // Communication loop: read and send messages back to the client
    while ((bytesRead = read(clientSocket, buffer, BUFFER_SIZE)) > 0) {
        buffer[bytesRead] = '\0';  // Null-terminate the message
        printf("Received: %s", buffer);
        send(clientSocket, buffer, bytesRead, 0);  // Echo the message back to the client
    }

    printf("Client disconnected.\n");
    close(clientSocket);  // Close the client socket
    exit(0);  // Terminate the child process
}

int main() {
    int uid;
    int arr[20];
    printf("Elements: %d\n",sizeof(arr)/sizeof(uid));

    return 0;

    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    // Install the SIGCHLD handler to prevent zombie processes
    signal(SIGCHLD, handle_sigchld);

    // Create the server socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind the socket to the specified address and port
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Bind failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(serverSocket, 5) == -1) {
        perror("Listen failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Server loop to accept incoming client connections
    while (1) {
        // Accept an incoming connection
        if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen)) == -1) {
            perror("Accept failed");
            continue;  // Try to accept the next connection
        }

        printf("New client connected.\n");

        // Create a new process using fork
        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            close(clientSocket);
            continue;
        }

        if (pid == 0) {
            // Child process: handle the client connection
            close(serverSocket);  // Close the server socket in the child process
            handleClient(clientSocket);
        } else {
            // Parent process: close the client socket and continue accepting new connections
            close(clientSocket);
        }
    }

    // Close the server socket (unreachable in this infinite loop)
    close(serverSocket);
    return 0;
}
