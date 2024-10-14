#ifndef BANK_DATA
#define BANK_DATA

typedef enum usertype{CUSTOMER,BANKER,MANAGER,ADMIN,NA} Usertype;

struct UserDetails{
    char username[128];
    char password[128];
    Usertype role;
};

#endif