#include"banking_client.h"
#include<stdio.h>

int main(){

    initBankingClient();

    // validateUser("hello_cs","hello_pwd");
    Usertype role = CUSTOMER;

    UserRecord data = {0,"hello_bk","pwd","Hello Client","hello.3@email.com",role,1};

    DataBlock dataBlock = {ADD_USER_ENTRY,2,0.0,0,NULL};
    copyToPayload(data,dataBlock);

    //free(queryBankingServer(&dataBlock));

    DataBlock d2block = {GET_USERS_BY_ROLE,getUserId(),100.0,0,NULL};
    copyToPayload(role,d2block);

    DataBlock *rspBlock = queryBankingServer(&d2block);

    UserRecord *records = (UserRecord*)rspBlock->payload;

    for(int i = 0; records[i].role == role; i++)
        printf("%d - %s %s- %d\n",records[i].userId,records[i].username,records[i].password,records[i].role); 



    // DataBlock block = {GET_BALANCE,getUserId(),0,0,NULL};

    // DataBlock *logs = queryBankingServer(&block);
    
    // char msg[128] = "";

    // snprintf(msg, 128, "Account Balance:\t%0.2f", logs->amount);
    // printf("%s",msg);

    // free(logs);

    // return 0;

    // DataBlock dataBlock = {DEPOSIT_CASH,getUserId(),101.0,0,NULL};
    // // Usertype role = CUSTOMER;
    // // copyToPayload(role,dataBlock);

    // DataBlock *rspBlock = queryBankingServer(&dataBlock);

    // DataBlock d2block = {GET_TXN_HISTORY,getUserId(),100.0,0,NULL};

    // rspBlock = queryBankingServer(&d2block);

    // TxnLogs *txnLogs = (TxnLogs*)rspBlock->payload;

    // for(int i = 0; txnLogs[i].userId > 0; i++){
    //     printf("%s\t:%0.2f\n", get_txn_type_name(txnLogs[i].transaction), txnLogs[i].txnAmount);
    // }

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