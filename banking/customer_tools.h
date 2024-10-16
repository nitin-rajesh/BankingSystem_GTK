#include"commons/bankstructs.h"
#ifndef CUSTOMER_TOOLS
#define CUSTOMER_TOOLS

int depositCash(int userId, double amount);

int withdrawCash(int userId, double amount);

int transferCash(int userId, int destUserId, double amount);

int applyForLoan(int userId, double amount);

int submitFeedback(int userId, char* feedback);

#endif