#include"commons/crud.h"
#include"configs/repo_loc.h"
#include<stdio.h>
#include<stdarg.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

DataBlock runBankingQuery(DataBlock dataBlock) {

    DataBlock blockToReturn;

    int nextUid;
    getNextId(USER_REPO,UserRecord,userId,nextUid);

    int nextLoanId;
    getNextId(LOAN_DATA,LoanData,loanId,nextLoanId);

    switch (dataBlock.crudOp) {
        case ADD_USER_ENTRY: {
            //Args: UserRecord recordToWrite
            UserRecord record;
            initPayload(UserRecord,record,dataBlock);

            record.userId = nextUid;
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
            TxnLogs logs[getArrSize(TXN_LOGS,TxnLogs)];
            readAllRecords(TXN_LOGS,userId,logs);

            int i = 0;
            while(logs[i].userId > 0){
                balance += logs[i].txnAmount;
            }
            blockToReturn.amount = balance;
            break;
        }
        case GET_TXN_HISTORY: {
            //Args: int userId, TxnLogs* txnLogsArr
            int userId = dataBlock.id;
            TxnLogs logs[getArrSize(TXN_LOGS,TxnLogs)];
            readAllRecords(TXN_LOGS,userId,logs); 
            copyArrToPayload(logs,blockToReturn);         
            break;
        }
        case GET_LOAN_DATA: {
            //Args: int userId, LoanData* loanDataArr
            int userId = dataBlock.id;
            LoanData logs[getArrSize(LOAN_DATA,LoanData)];
            readAllRecords(LOAN_DATA,userId,logs); 
            copyArrToPayload(logs,blockToReturn);           
            break;
        }
        case GET_USER_RECORD: {
            //Args: int userId, UserRecord* record
            int userId = dataBlock.id;
            UserRecord data;
            readRecord(USER_REPO,userId,data);
            copyToPayload(data,blockToReturn);
            break;
        }
        case GET_USERS_BY_ROLE: {
            //Args: UserType role, UserRecord* recordsArr
            Usertype role;
            initPayload(Usertype,role,dataBlock);
            UserRecord userArr[getArrSize(USER_REPO,UserRecord)];
            readAllRecords(USER_REPO,role,userArr);
            copyArrToPayload(userArr,blockToReturn);
            break;
        }
        case GET_USER_BY_NAME: {
            //Args: char* username, UserRecord* record
            char* username = (char*)dataBlock.payload;
            UserRecord userArr[getArrSize(USER_REPO,UserRecord)];
            boolean isActive = ACTIVE;
            readAllRecords(USER_REPO,isActive,userArr);
            int i = 0;
            while(userArr[i].isActive > 0){
                if(strcmp(userArr[i].username,username)==0)
                    break;
                i++;
            }
            UserRecord temp = userArr[i];
            copyToPayload(temp,blockToReturn);
            break;
        } 
        case DEPOSIT_CASH: {
            //Args: int userId, double amountToDeposit
            int userId = dataBlock.id;
            double amount = dataBlock.amount;
            TxnLogs data = {userId,amount,time(NULL),DEPOSIT};
            writeRecord(TXN_LOGS,data);
            break;
        }
        case WITHDRAW_CASH: {
            //Args: int userId, double amountToWithdraw
            int userId = dataBlock.id;
            double amount = dataBlock.amount;
            TxnLogs data = {userId,-amount,time(NULL),WITHDRAWAL};
            writeRecord(TXN_LOGS,data);
            break;
        }
        case TRANSFER_CASH: {
            //Args: int userId, int destUserId, double amountToWithdraw
            int userId = dataBlock.id;
            double amount = dataBlock.amount;            
            int destUserId;
            initPayload(int,destUserId,dataBlock);
            TxnLogs data = {userId,-amount,time(NULL),TRANSFER};
            writeRecord(TXN_LOGS,data);    
            userId = destUserId;
            TxnLogs newdata = {userId,amount,time(NULL),DEPOSIT};
            writeRecord(TXN_LOGS,newdata);
            break;
        }
        case APPLY_FOR_LOAN: {
            //Args: int userId, double loanAmount
            int userId = dataBlock.id;
            double amount = dataBlock.amount;            
            double loanId = 0;
            getNextId(LOAN_DATA,LoanData,loanId,loanId);
            LoanData data = {userId,loanId,0,amount,0,INACTIVE};
            writeRecord(LOAN_DATA,data);    
            break;
        }
        case SUBMIT_FEEDBACK: {
            //Args: int userId, char* feedback;
            int userId = dataBlock.id;
            char feedback[1024];
            strcpy(feedback,(char*)dataBlock.payload);
            FeedBack data = {userId,feedback,INACTIVE};
            writeRecord(FEEDBACK,data);    
            break;
        }
        case ACTIVATE_USER: {
            //Args: int userId;
            int userId = dataBlock.id;
            UserRecord data;
            readRecord(USER_REPO,userId,data);
            data.isActive = ACTIVE;
            writeRecordAt(USER_REPO,userId,data,UserRecord);
            break;
        }
        case DEACTIVATE_USER: {
            //Args: int userId;
            int userId = dataBlock.id;
            UserRecord data;
            readRecord(USER_REPO,userId,data);
            data.isActive = INACTIVE;
            writeRecordAt(USER_REPO,userId,data,UserRecord);
            break;
        }
        case ASSIGN_BANKER_FOR_LOAN: {
            //Args: int loanId, int bankerId
            int loanId = dataBlock.id;
            int bankerId;
            initPayload(int,bankerId,dataBlock);
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
    return blockToReturn;
}
