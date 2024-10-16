#include"commons/bankstructs.h"
#ifndef COMMON_TOOLS
#define COMMON_TOOLS

#define USER_REPO "banking/db/user_repo.bin"
#define ACCOUNT_REPO "banking/db/account_repo.bin"
#define TXN_LOGS "banking/db/txn_logs.bin"
#define LOAD_DATA "banking/db/load_data.bin"
#define FEEDBACK "banking/db/feedback.bin"

double getBalance(int userId);

TxnLogs getTxnHistory(int userId);

LoanData getLoanData(int userId);

UserRecord getUserRecord(int userId);

int getUserIdFromName(char* username);

#endif