#include"banking/banktools_handler.h"
#include"local_files.h"
#include<stdio.h>

int main(){

    Usertype role = CUSTOMER;
    
    UserRecord data = {0,"hello_db","hello_pwd","Hello From Crud","hello.3@email.com",role,1};

    DataBlock dataBlock = {ADD_USER_ENTRY,0,0.0,NULL};
    copyToPayload(data,dataBlock);

    runBankingQuery(dataBlock);

    dataBlock.crudOp = GET_USERS_BY_ROLE;
    copyToPayload(role,dataBlock);
    DataBlock retBlock = runBankingQuery(dataBlock);

    UserRecord *records = (UserRecord*)retBlock.payload;

    for(int i = 0; records[i].role == role; i++){
        printf("%d - %s - %d\n",records[i].userId,records[i].fullname,records[i].role);
    }

}