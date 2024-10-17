#include"commons/bankstructs.h"
#ifndef BANKER_TOOLS
#define BANKER_TOOLS


int addCustomer(UserRecord record);

int modifyCustomer(int userId, UserRecord newRecord);

int processLoan(int userId, int loadId);

#endif