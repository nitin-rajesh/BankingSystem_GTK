#ifndef BANKENUMS
#define BANKENUMS

typedef short int boolean;

typedef enum usertype{CUSTOMER=1,BANKER,MANAGER,ADMIN,NA} Usertype;

typedef enum txntype{WITHDRAWAL=1,DEPOSIT,LOAN_REQ,LOAN_PAYOFF,TRANSFER} TxnType;

typedef enum structtype{
    STRUCT_USER_DETAILS=1,
    STRUCT_USER_RECORD,
    STRUCT_ACCOUNT_BALANCE,
    STRUCT_TXN_LOGS,
    STRUCT_LOAN_DATA,
    STRUCT_FEEDBACK
}StructType;

typedef enum functiontype{
    ADD_USER_ENTRY=1,
    MODIFY_USER_ENTRY,
    ADD_CUSTOMER,
    MODIFY_CUSTOMER,
    PROCESS_LOAN,
    GET_REC_SIZE,
    GET_BALANCE,
    GET_TXN_HISTORY,
    GET_LOAN_DATA,
    GET_ACTIVE_LOANS,
    GET_USER_RECORD,
    GET_USERS_BY_ROLE,
    GET_USER_BY_NAME,
    DEPOSIT_CASH,
    WITHDRAW_CASH,
    TRANSFER_CASH,
    APPLY_FOR_LOAN,
    SUBMIT_FEEDBACK,
    ACTIVATE_USER,
    DEACTIVATE_USER,
    ASSIGN_BANKER_FOR_LOAN
} FunctionType;

const char* get_txn_type_name(TxnType txn) {
    switch (txn) {
        case WITHDRAWAL:
            return "WITHDRAWAL";
        case DEPOSIT:
            return "DEPOSIT";
        case LOAN_REQ:
            return "LOAN_REQ";
        case LOAN_PAYOFF:
            return "LOAN_PAYOFF";
        case TRANSFER:
            return "TRANSFER";
        default:
            return "UNKNOWN";
    }
}

const char* get_usertype_name(Usertype role) {
    switch (role) {
        case CUSTOMER:
            return "CUSTOMER";
        case BANKER:
            return "BANKER";
        case MANAGER:
            return "MANAGER";
        case ADMIN:
            return "ADMIN";
        case NA:
            return "N/A";
        default:
            return "UNKNOWN";
    }
}

#endif