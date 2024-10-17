#include"commons/bankstructs.h"
#include"admin_tools.h"
#include"banker_tools.h"
#include"customer_tools.h"
#include"manager_tools.h"
#include<sys/stat.h>

#ifndef COMMON_TOOLS
#define COMMON_TOOLS


long getRecSize(char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0)
        return st.st_size;
    else 
        return 0;
    
}

double getBalance(char* filename, int userId){
    AccountBalance bal;
    readRecord(filename,userId,bal);

    if(bal.isActive)
        return bal.accBalance;
    
    return 0.0;
}

TxnLogs* getTxnHistory(char* filename, int userId){
    TxnLogs* txnLogList = malloc(getRecSize(filename));
    readAllRecords(filename,userId,txnLogList);
}

LoanData getLoanData(int userId);

UserRecord getUserRecord(int userId);

int getUserIdFromName(char* username);

#endif