#include"banking_client.h"
#include<stdio.h>

int main(){

    initBankingClient();

    validateUser("hello_cs","hello_pwd");

    DataBlock dataBlock = {GET_USERS_BY_ROLE,0,0.0,0,NULL};
    Usertype role = CUSTOMER;
    copyToPayload(role,dataBlock);

    DataBlock* rspBlock = queryBankingServer(&dataBlock);

    UserRecord *records = (UserRecord*)rspBlock->payload;

    for(int i = 0; records[i].role > 0; i++){
        printf("%d - %s - %d\n",records[i].userId,records[i].fullname,records[i].role);
    }

    // Usertype role = CUSTOMER;
    
    // UserRecord data = {0,"hello_db","hello_pwd","Hello From Crud","hello.3@email.com",role,1};

    // DataBlock dataBlock = {ADD_USER_ENTRY,0,0.0,NULL};

    // printf("%ld before assignment\n",getBlockSize(dataBlock));

    // copyToPayload(data,dataBlock);

    // printf("%ld after assignment\n",getBlockSize(dataBlock));

    // runBankingQuery(dataBlock);

    // dataBlock.crudOp = GET_USERS_BY_ROLE;
    // copyToPayload(role,dataBlock);
    // DataBlock retBlock = runBankingQuery(dataBlock);

    // UserRecord *records = (UserRecord*)retBlock.payload;

    // for(int i = 0; records[i].role == role; i++){
    //     printf("%d - %s - %d\n",records[i].userId,records[i].fullname,records[i].role);
    // }

}