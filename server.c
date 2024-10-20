#include"banking/network_handler.h"
#include"banking/network/server_tools.h"
#include"banking/banktools_handler.h"

#define PORT 6066
#define BUFFER_SIZE 1048576

// Function to handle client communication
void handleClientQuery(int clientSocket){
    DataBlock* dataBlock = (DataBlock*)malloc(sizeof(DataBlock));
    size_t blocksize;

    printf("Handling new client...\n");

    // Communication loop: read and send messages back to the client
    while ((blocksize = receiveDataBlock(clientSocket, dataBlock)) > 0) {
        
        printf("Crud op received: %d\n",dataBlock->crudOp);
        DataBlock retblock = runBankingQuery(*dataBlock);

        printf("Returning data of size %ld\n", getBlockSize(retblock));

        sendDataBlock(clientSocket, &retblock); 
    }

    printf("Client disconnected.\n");
    close(clientSocket);  // Close the client socket
    exit(0);  // Terminate the child process
}


int main(){
    int serversock, backlog = 64;
    printf("Starting server\n");

    initializeServer(PORT,backlog,&serversock);
    printf("Initialized server\n");
    acceptConnectionAtFork(serversock);

    return 0;
}
