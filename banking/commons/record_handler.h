#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

#ifndef RECORD_HANDLER
#define RECORD_HANDLER

#define writeRecord(filename, data) ({        \
    int fd = open(filename,O_CREAT|O_WRONLY|O_APPEND, 0644); \
    if(fd < 0)                                \
        return -1;                            \
    write(fd,&data,sizeof(data));             \
    close(fd);                                \
})

#define readRecord(filename, uid, data) ({     \
    int fd = open(filename,O_RDONLY, 0644);    \
    int cid = -1, bytesin = 1;                 \
    while(cid != uid && bytesin > 0){          \
        bytesin = read(fd,&data,sizeof(data)); \
        cid = data.userId;                     \
    }                                          \
})

#define readLastRecord(filename, data) ({   \
    int fd = open(filename,O_RDONLY, 0644); \
    lseek(fd,-sizeof(data),SEEK_END);       \
    read(fd,&data,sizeof(data));            \
})

#define getNextId(filename, recordType, idType, nextId) ({  \
    recordType temp;                        \
    readLastRecord(filename,temp);          \
    nextId = temp.idType + 1;               \
})

#endif