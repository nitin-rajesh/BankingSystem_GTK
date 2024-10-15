#include<sys/time.h>
#ifndef BANK_DATA
#define BANK_DATA

typedef short int boolean;

typedef enum usertype{CUSTOMER,BANKER,MANAGER,ADMIN,NA} Usertype;

typedef enum txntype{WITHDRAWAL,DEPOSIT,LOAN_REQ,LOAN_PAYOFF,TRANSFER} TxnType;

struct UserDetails{
    char username[128];
    char password[128];
    Usertype role;
};

struct UserRecord{
    int userId;
    char username[128];
    char password[128];
    char fullname[128];
    char emailId[64];
    Usertype role;
    boolean isActive;
};

struct AccountBalance{
    int userId;
    double accBalance;
    boolean isActive;
};

struct TxnData{
    int userId;
    double txnAmount;
    TxnType transaction;
};

struct LoadData{
    double loanRequest;
    double loanPaid;
    boolean loanApproved;
};

struct FeedBack{
    int userId;
    char feedBack[1024];
    boolean isRead;
};

#endif