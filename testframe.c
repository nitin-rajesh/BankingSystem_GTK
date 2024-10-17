#include"banking/banktools_handler.h"
#include"local_files.h"
#include<stdio.h>

int main(){
    int nextUid;
    getNextId(USER_REPO,UserRecord,userId,nextUid);

    Usertype role = CUSTOMER;

    UserRecord data = {nextUid,"hello_func","hello_pwd","Hello From Query","hello.3@email.com",role,1};

    runBankingQuery(ADD_USER_ENTRY,data);

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