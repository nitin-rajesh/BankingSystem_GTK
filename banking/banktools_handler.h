#include"commons/common_tools.h"
#include<stdio.h>
#include<stdarg.h>
#include<stdlib.h>
#include<time.h>

#define USER_REPO "banking/db/user_repo.bin"
#define ACCOUNT_REPO "banking/db/account_repo.bin"
#define TXN_LOGS "banking/db/txn_logs.bin"
#define LOAN_DATA "banking/db/load_data.bin"
#define FEEDBACK "banking/db/feedback.bin"

typedef enum {
    ADD_USER_ENTRY=1,
    MODIFY_USER_ENTRY,
    ADD_CUSTOMER,
    MODIFY_CUSTOMER,
    PROCESS_LOAN,
    GET_REC_SIZE,
    GET_BALANCE,
    GET_TXN_HISTORY,
    GET_LOAN_DATA,
    GET_USER_RECORD,
    GET_USERS_BY_ROLE,
    DEPOSIT_CASH,
    WITHDRAW_CASH,
    TRANSFER_CASH,
    APPLY_FOR_LOAN,
    SUBMIT_FEEDBACK,
    ACTIVATE_USER,
    DEACTIVATE_USER,
    ASSIGN_BANKER_FOR_LOAN
} FunctionType;

// Switch-case to call appropriate functions
void runBankingQuery(FunctionType funcType, ...) {
    va_list ptr;
    va_start(ptr,funcType);

    switch (funcType) {
        case ADD_USER_ENTRY: {
            //Args: UserRecord recordToWrite
            UserRecord record = va_arg(ptr,UserRecord);
            writeRecord(USER_REPO,record);
            break;
        }
        case MODIFY_USER_ENTRY: {
            //Args: int existingUserId, UserRecord recordToWrite
            int userId = va_arg(ptr,int);
            UserRecord newRecord = va_arg(ptr,UserRecord);
            writeRecordAt(USER_REPO,userId,newRecord,UserRecord);
            break;
        }
        case PROCESS_LOAN: {
            //Args: int loanId
            int loanId = va_arg(ptr,int);
            LoanData data;
            readRecord(LOAN_DATA,loanId,data);

            if(data.loanApproved == INACTIVE){
                data.loanApproved = ACTIVE;
                TxnLogs loanBalance = {data.userId,
                    data.loanRequest, time(NULL), LOAN_REQ};
                writeRecord(TXN_LOGS,loanBalance);
            }
            break;
        }
        case GET_BALANCE: {
            //Args: int userId, double* balanceAmtVar
            int userId = va_arg(ptr,int);
            double* balance = va_arg(ptr,double*);
            TxnLogs* logs = malloc(getRecSize(TXN_LOGS));
            readAllRecords(TXN_LOGS,userId,logs);

            *balance = 0;
            int i = 0;
            while(logs[i].userId > 0){
                *balance += logs[i].txnAmount;
            }
            break;
        }
        case GET_TXN_HISTORY: {
            //Args: int userId, TxnLogs* txnLogsArr
            int userId = va_arg(ptr,int);
            TxnLogs* logs = va_arg(ptr,TxnLogs*);
            logs = malloc(getRecSize(TXN_LOGS));
            readAllRecords(TXN_LOGS,userId,logs);            
            break;
        }
        case GET_LOAN_DATA: {
            //Args: int userId, LoanData* loanDataArr
            int userId = va_arg(ptr,int);
            LoanData* logs = va_arg(ptr,LoanData*);
            logs = malloc(getRecSize(LOAN_DATA));
            readAllRecords(LOAN_DATA,userId,logs);            
            break;
        }
        case GET_USER_RECORD: {
            //Args: int userId, UserRecord* record
            int userId = va_arg(ptr,int);
            UserRecord* userRec = va_arg(ptr,UserRecord*);
            userRec = malloc(sizeof(UserRecord));
            UserRecord data;
            readRecord(USER_REPO,userId,data);
            *userRec = data;
            break;
        }
        case GET_USERS_BY_ROLE: {
            //Args: UserType role, UserRecord* recordsArr
            Usertype role = va_arg(ptr,Usertype);
            UserRecord* userArr = va_arg(ptr,UserRecord*);
            userArr = malloc(sizeof(UserRecord));
            readAllRecords(USER_REPO,role,userArr);
            break;
        }
        case DEPOSIT_CASH: {
            //Args: int userId, double amountToDeposit
            int userId = va_arg(ptr,int);
            double amount = va_arg(ptr,double);
            TxnLogs data = {userId,amount,time(NULL),DEPOSIT};
            writeRecord(TXN_LOGS,data);
            break;
        }
        case WITHDRAW_CASH: {
            //Args: int userId, double amountToWithdraw
            int userId = va_arg(ptr,int);
            double amount = va_arg(ptr,double);
            TxnLogs data = {userId,-amount,time(NULL),WITHDRAWAL};
            writeRecord(TXN_LOGS,data);
            break;
        }
        case TRANSFER_CASH: {
            //Args: int userId, int destUserId, double amountToWithdraw
            int userId = va_arg(ptr,int);
            int destUserId = va_arg(ptr,int);
            double amount = va_arg(ptr,double);
            TxnLogs data = {userId,-amount,time(NULL),TRANSFER};
            writeRecord(TXN_LOGS,data);    
            userId = destUserId;
            TxnLogs data = {userId,amount,time(NULL),DEPOSIT};
            writeRecord(TXN_LOGS,data);
            break;
        }
        case APPLY_FOR_LOAN: {
            //Args: int userId, double loanAmount
            int userId = va_arg(ptr,int);
            double amount = va_arg(ptr,double);
            double loanId = 0;
            getNextId(LOAN_DATA,LoanData,loanId,loanId);
            LoanData data = {userId,loanId,0,amount,0,INACTIVE};
            writeRecord(LOAN_DATA,data);    
            break;
        }
        case SUBMIT_FEEDBACK: {
            //Args: int userId, char* feedback;
            int userId = va_arg(ptr,int);
            char* feedback = va_arg(ptr,char*);
            FeedBack data = {userId,feedback,INACTIVE};
            writeRecord(FEEDBACK,data);    
            break;
        }
        case ACTIVATE_USER: {
            //Args: int userId, char* feedback;
            int userId = va_arg(ptr,int);
            UserRecord data;
            readRecord(USER_REPO,userId,data);
            data.isActive = ACTIVE;
            writeRecordAt(USER_REPO,userId,data,UserRecord);
            break;
        }
        case DEACTIVATE_USER: {
            //Args: int userId, char* feedback;
            int userId = va_arg(ptr,int);
            UserRecord data;
            readRecord(USER_REPO,userId,data);
            data.isActive = INACTIVE;
            writeRecordAt(USER_REPO,userId,data,UserRecord);
            break;
        }
        case ASSIGN_BANKER_FOR_LOAN: {
            //Args: int loanId, int bankerId
            int loanId = va_arg(ptr,int);
            int bankerId = va_arg(ptr,int);
            LoanData data;
            readRecord(LOAN_DATA,loanId,data);
            data.assignedBankerId = bankerId;
            writeRecordAt(LOAN_DATA,loanId,data,LoanData);
            break;
        }
        default:
            printf("Invalid function type\n");
            break;
    }
}
