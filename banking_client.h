#ifndef GLOBAL_CONFIGS
#define GLOBAL_CONFIGS
#include"banking/network_handler.h"

#define SERVER_IP "127.0.0.1"  // Server's IP address (localhost)
#define SERVER_PORT 6066        // Server's port
#define BUFFER_SIZE 1048576        // Buffer size for communication

static int clientSocket;

static int sessionUserId;

DataBlock dataBlock;
DataBlock rspBlock;

int validateUser(const char *username, const char *password){
    sessionUserId = 0;

    dataBlock.crudOp=GET_USER_BY_NAME;

    char unameArr[strlen(username)];
    strcpy(unameArr,username);
    copyArrToPayload(unameArr,dataBlock);

    sendDataBlock(clientSocket,&dataBlock);

    receiveDataBlock(clientSocket,&rspBlock);

    UserRecord *rec = (UserRecord*)rspBlock.payload;

    if(rec && rec->userId > 0 && strcmp(rec->password,password) == 0 && rec->isActive){
        sessionUserId = rec->userId;
        return rec->role;
    }

    return 0;
}

int initBankingClient(){
    return initializeClient(SERVER_IP,SERVER_PORT,&clientSocket);
}

DataBlock* queryBankingServer(DataBlock *dataBlock){
    DataBlock *rspBlock = malloc(sizeof(DataBlock));

    sendDataBlock(clientSocket,dataBlock);

    if(receiveDataBlock(clientSocket,rspBlock) <= 0){
        printf("Failed to receive response from server\n");
    }

    return rspBlock;
}

int getUserId(){
    return sessionUserId;
}

#endif