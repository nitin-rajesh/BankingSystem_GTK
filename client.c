#include"banking/network_handler.h"

#define SERVER_IP "127.0.0.1"  // Server's IP address (localhost)
#define SERVER_PORT 6066        // Server's port
#define BUFFER_SIZE 1048576        // Buffer size for communication

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    initializeClient(SERVER_IP,SERVER_PORT,&clientSocket);

    Usertype role = CUSTOMER;

    UserRecord data = {0,"hello_cs","hello_pwd","Hello From Client","hello.3@email.com",role,1};

    DataBlock dataBlock = {ADD_USER_ENTRY,0,0.0,0,NULL};
    copyToPayload(data,dataBlock);

    DataBlock retBlock;

    sendDataBlock(clientSocket,&dataBlock);

    // Receive the server's response
    receiveDataBlock(clientSocket,&retBlock);

    dataBlock.crudOp = GET_USERS_BY_ROLE;
    copyToPayload(role,dataBlock);

    sendDataBlock(clientSocket,&dataBlock);
    // Receive the server's response
    receiveDataBlock(clientSocket,&retBlock);

    UserRecord *records = (UserRecord*)retBlock.payload;

    for(int i = 0; records[i].role == role; i++)
        printf("%d - %s - %d\n",records[i].userId,records[i].username,records[i].role); 

    close(clientSocket);
    return 0;
}
