#include"banking/common_tools.h"
#include"local_files.h"
#include<stdio.h>

int main(){
    int nextUid;
    getNextId(USER_REPO,UserRecord,userId,nextUid);

    Usertype role = ADMIN;

    UserRecord data = {nextUid,"hello_again","hello_pwd","Hello Again","hello.3@email.com",role,1};

    writeRecord(USER_REPO,data);

    readLastRecord(USER_REPO,data);

    UserRecord *records = malloc(getRecSize(USER_REPO));

    readAllRecords(USER_REPO,role,records);

    for(int i = 0; records[i].role == role; i++){
        printf("%d - %s - %d\n",records[i].userId,records[i].fullname,records[i].role);
    }

    //printf("\nBalance: %lf\n",balance);

}