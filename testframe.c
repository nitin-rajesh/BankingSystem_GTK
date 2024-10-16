#include"banking/common_tools.h"
#include<stdio.h>

int main(){
    int nextUid;
    getNextId(USER_REPO,UserRecord,userId,nextUid);
    UserRecord data = {nextUid,"hello_again","hello_pwd","Hello Again","hello.3@email.com",ADMIN,1};

    writeRecord(USER_REPO,data);

    readLastRecord(USER_REPO,data);

    printf("%d\n%s\n%s",data.userId,data.fullname,data.emailId);

}