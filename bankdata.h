#ifndef BANK_DATA
#define BANK_DATA

enum usertype{Customer,Banker,Manager,Admin};

struct UserDetails{
    char username[128];
    char password[128];
    usertype role;
};

#endif