#include<sys/time.h>
#include"record_handler.h"
#include"bankenums.h"
#ifndef BANK_DATA
#define BANK_DATA

#define ACTIVE 1
#define INACTIVE 0


typedef struct userdetails{
    char username[128];
    char password[128];
    Usertype role;
}UserDetails;

typedef struct userrecord{
    int userId;
    char username[128];
    char password[128];
    char fullname[128];
    char emailId[64];
    Usertype role;
    boolean isActive;
} UserRecord;

typedef struct accountbalance{
    int userId;
    double accBalance;
    boolean isActive;
}AccountBalance;

typedef struct txnlogs{
    int userId;
    double txnAmount;
    time_t txnTimestamp;
    TxnType transaction;
}TxnLogs;

typedef struct loandata{
    int userId;
    int loanId;
    int assignedBankerId;
    double loanRequest;
    double loanPaid;
    boolean loanApproved;
}LoanData;

typedef struct feedback{
    int userId;
    char feedBack[1024];
    boolean isRead;
}FeedBack;

#endif