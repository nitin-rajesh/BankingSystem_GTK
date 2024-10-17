#include"bankstructs.h"
#include<sys/stat.h>

#ifndef COMMON_TOOLS
#define COMMON_TOOLS

// Admin
int addUserEntry(UserRecord record);

int modifyUserEntry(int userId, UserRecord newRecord);

// Banker
int addCustomer(UserRecord record);

int modifyCustomer(int userId, UserRecord newRecord);

int processLoan(int userId, int loanId);

// Getter tools
double getBalance(char* filename, int userId);

TxnLogs* getTxnHistory(char* filename, int userId);

LoanData getLoanData(int userId);

UserRecord getUserRecord(int userId);

int getUserIdFromName(char* username);

// Customer
int depositCash(int userId, double amount);

int withdrawCash(int userId, double amount);

int transferCash(int userId, int destUserId, double amount);

int applyForLoan(int userId, double amount);

int submitFeedback(int userId, char* feedback);

// Manager
int activateCustomer(int userId);

int deactivateCustomer(int userId);

int assignBankerForLoan(int bankerUId);

// Generic
long getRecSize(char *filename){
    struct stat fileStat;

    if (stat(filename, &fileStat) == -1) 
        return -1;  
        
    return fileStat.st_size;
}

#endif