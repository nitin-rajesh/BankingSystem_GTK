#include"commons/crud.h"
#include<stdio.h>
#include<stdarg.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#define USER_REPO "db/user_repo.bin"
#define ACCOUNT_REPO "db/account_repo.bin"
#define TXN_LOGS "db/txn_logs.bin"
#define LOAN_DATA "db/load_data.bin"
#define FEEDBACK "db/feedback.bin"


// Switch-case to call appropriate functions
DataBlock runBankingQuery(DataBlock dataBlock,...) {
    va_list ptr;
    va_start(ptr,dataBlock);

    DataBlock returnData;

    int nextUid;
    getNextId(USER_REPO,UserRecord,userId,nextUid);

    int nextLoanId;
    getNextId(LOAN_DATA,LoanData,loanId,nextUid);


    switch (dataBlock.crudOp) {
        case ADD_USER_ENTRY: {
            //Args: UserRecord recordToWrite
            UserRecord record;
            initPayload(UserRecord,record,dataBlock);
            record.userId = nextUid+1;
            writeRecord(USER_REPO,record);
            break;
        }
        case MODIFY_USER_ENTRY: {
            //Args: int existingUserId, UserRecord recordToWrite
            int userId = dataBlock.id;
            UserRecord newRecord;
            initPayload(UserRecord,newRecord,dataBlock);
            writeRecordAt(USER_REPO,userId,newRecord,UserRecord);
            break;
        }
        case PROCESS_LOAN: {
            //Args: int loanId
            int loanId = dataBlock.id;
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
            int userId = dataBlock.id;
            double balance = 0.0;
            TxnLogs* logs = malloc(getRecSize(TXN_LOGS));
            readAllRecords(TXN_LOGS,userId,logs);

            int i = 0;
            while(logs[i].userId > 0){
                balance += logs[i].txnAmount;
            }
            returnData.amount = balance;
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
        case GET_USER_BY_NAME: {
            //Args: char* username, UserRecord* record
            char* username = va_arg(ptr,char*);
            UserRecord* userRec = va_arg(ptr,UserRecord*);
            UserRecord* userArr = malloc(sizeof(UserRecord));
            boolean isActive = ACTIVE;
            readAllRecords(USER_REPO,isActive,userArr);
            int i = 0;
            while(userArr[i].isActive > 0){
                if(strcmp(userArr[i].username,username)==0)
                    break;
                i++;
            }
            *userRec = userArr[i];
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
            TxnLogs newdata = {userId,amount,time(NULL),DEPOSIT};
            writeRecord(TXN_LOGS,newdata);
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
            //Args: int userId;
            int userId = va_arg(ptr,int);
            UserRecord data;
            readRecord(USER_REPO,userId,data);
            data.isActive = ACTIVE;
            writeRecordAt(USER_REPO,userId,data,UserRecord);
            break;
        }
        case DEACTIVATE_USER: {
            //Args: int userId;
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
