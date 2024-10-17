#include"commons/common_tools.h"
#include<stdio.h>
#include<stdarg.h>
#include<stdlib.h>

#define USER_REPO "banking/db/user_repo.bin"
#define ACCOUNT_REPO "banking/db/account_repo.bin"
#define TXN_LOGS "banking/db/txn_logs.bin"
#define LOAD_DATA "banking/db/load_data.bin"
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
    GET_USER_ID_FROM_NAME,
    DEPOSIT_CASH,
    WITHDRAW_CASH,
    TRANSFER_CASH,
    APPLY_FOR_LOAN,
    SUBMIT_FEEDBACK,
    ACTIVATE_CUSTOMER,
    DEACTIVATE_CUSTOMER,
    ASSIGN_BANKER_FOR_LOAN
} FunctionType;

// Switch-case to call appropriate functions
void runBankingQuery(FunctionType funcType, ...) {
    va_list ptr;

    switch (funcType) {
        case ADD_USER_ENTRY: {
            va_start(ptr,funcType);
            UserRecord record = va_arg(ptr,UserRecord);
            writeRecord(USER_REPO,record);
            break;
        }
        case MODIFY_USER_ENTRY: {
            UserRecord newRecord;
            printf("modifyUserEntry result: %d\n", modifyUserEntry(1, newRecord));
            break;
        }
        case ADD_CUSTOMER: {
            UserRecord record;
            printf("addCustomer result: %d\n", addCustomer(record));
            break;
        }
        case MODIFY_CUSTOMER: {
            UserRecord newRecord;
            printf("modifyCustomer result: %d\n", modifyCustomer(2, newRecord));
            break;
        }
        case PROCESS_LOAN: {
            printf("processLoan result: %d\n", processLoan(1, 101));
            break;
        }
        case GET_REC_SIZE: {
            printf("getRecSize result: %ld\n", getRecSize("records.dat"));
            break;
        }
        case GET_BALANCE: {
            printf("getBalance result: %.2f\n", getBalance("users.dat", 1));
            break;
        }
        // case GET_TXN_HISTORY: {
        //     TxnLogs* logs = getTxnHistory("transactions.dat", 1);
        //     printf("getTxnHistory result: %s\n", logs->details);
        //     break;
        // }
        // case GET_LOAN_DATA: {
        //     LoanData loan = getLoanData(1);
        //     printf("getLoanData result: %.2f\n", loan.amount);
        //     break;
        // }
        // case GET_USER_RECORD: {
        //     UserRecord user = getUserRecord(1);
        //     printf("getUserRecord result: %s\n", user.name);
        //     break;
        // }
        case GET_USER_ID_FROM_NAME: {
            printf("getUserIdFromName result: %d\n", getUserIdFromName("Alice"));
            break;
        }
        case DEPOSIT_CASH: {
            printf("depositCash result: %d\n", depositCash(1, 1000.0));
            break;
        }
        case WITHDRAW_CASH: {
            printf("withdrawCash result: %d\n", withdrawCash(1, 500.0));
            break;
        }
        case TRANSFER_CASH: {
            printf("transferCash result: %d\n", transferCash(1, 2, 1000.0));
            break;
        }
        case APPLY_FOR_LOAN: {
            printf("applyForLoan result: %d\n", applyForLoan(1, 20000.0));
            break;
        }
        case SUBMIT_FEEDBACK: {
            printf("submitFeedback result: %d\n", submitFeedback(1, "Great service!"));
            break;
        }
        case ACTIVATE_CUSTOMER: {
            printf("activateCustomer result: %d\n", activateCustomer(2));
            break;
        }
        case DEACTIVATE_CUSTOMER: {
            printf("deactivateCustomer result: %d\n", deactivateCustomer(2));
            break;
        }
        case ASSIGN_BANKER_FOR_LOAN: {
            printf("assignBankerForLoan result: %d\n", assignBankerForLoan(10));
            break;
        }
        default:
            printf("Invalid function type\n");
            break;
    }
}
