#include"bankstructs.h"

#ifndef NETWORK_STRUCTS
#define NETWORK_STRUCTS

#define initPayload(typename, varname, dblock) ({  \
    typename* temp = (typename*) dblock.payload;   \
    varname = *temp;                               \
})

#define copyToPayload(varname, dblock) ({            \
    dblock.payload = malloc(sizeof(varname));        \
    memcpy(dblock.payload,&varname,sizeof(varname)); \
})

#define copyArrToPayload(varname, dblock) ({   \
    dblock.payload = malloc(sizeof(varname));  \
    memcpy(dblock.payload,varname,sizeof(varname));       \
})


typedef struct dataBlock{
    FunctionType crudOp;
    int id;
    double amount;
    void* payload;
}DataBlock;

#endif