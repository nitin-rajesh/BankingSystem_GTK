#include"banking/banktools_handler.h"
#include"local_files.h"
#include<stdio.h>

int main(){

    Usertype role = ADMIN;

    UserRecord data = {0,"hello_db","hello_pwd","Hello From Crud","hello.3@email.com",role,1};

    DataBlock dataBlock = {ADD_USER_ENTRY,0,0.0,NULL};
    copyToPayload(data,dataBlock);

    runBankingQuery(dataBlock);

    // writeRecord(USER_REPO,data);

    // readLastRecord(USER_REPO,data);

    // int userId = data.userId;

    // UserRecord newdata = {userId,"hi_lolz","hello_pwd","New Hi","hello.3@email.com",role,1};

    // writeRecordAt(USER_REPO,userId,newdata,UserRecord);

    UserRecord *records = malloc(getRecSize(USER_REPO));

    readAllRecords(USER_REPO,role,records);

    for(int i = 0; records[i].role == role; i++){
        printf("%d - %s - %d\n",records[i].userId,records[i].fullname,records[i].role);
    }

    //printf("\nBalance: %lf\n",balance);

}