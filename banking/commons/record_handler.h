#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/file.h>
#include<sys/stat.h>

#ifndef RECORD_HANDLER
#define RECORD_HANDLER

#define writeRecord(filename, data) ({        \
    int fd = open(filename,O_CREAT|O_WRONLY|O_APPEND, 0644); \
    flock(fd,LOCK_EX);                        \
    write(fd,&data,sizeof(data));             \
    flock(fd,LOCK_UN);                        \
    close(fd);                                \
})

#define writeRecordAt(filename, uid, newdata, dtype) ({    \
    dtype ddata;                               \
    int fd = open(filename,O_RDWR, 0644);      \
    flock(fd,LOCK_EX);                         \
    int cid = -1, bytesin = 1;                 \
    while(cid != uid && bytesin > 0){          \
        bytesin = read(fd,&ddata,sizeof(ddata)); \
        cid = ddata.uid;                        \
    }                                          \
    lseek(fd,-sizeof(ddata),SEEK_CUR);          \
    write(fd,&newdata,sizeof(newdata));         \
    flock(fd,LOCK_UN);                         \
    close(fd);                                 \
})

#define readRecord(filename, uid, data) ({     \
    int fd = open(filename,O_RDONLY, 0644);    \
    flock(fd,LOCK_SH);                         \
    int cid = -1, bytesin = 1;                 \
    while(cid != uid && bytesin > 0){          \
        bytesin = read(fd,&data,sizeof(data)); \
        cid = data.uid;                        \
    }                                          \
    flock(fd,LOCK_UN);                         \
    close(fd);                                 \
})

#define readAllRecords(filename, uid, dataArr) ({     \
    int fd = open(filename,O_RDONLY, 0644);    \
    flock(fd,LOCK_SH);                         \
    int cid = -1, bytesin = 1, i = 0;          \
    while(bytesin > 0){                        \
        bytesin = read(fd,&dataArr[i],sizeof(dataArr[i])); \
        if(dataArr[i].uid == uid) i++;         \
    }                                          \
    dataArr[i].uid = -1;                       \
    flock(fd,LOCK_UN);                         \
    close(fd);                                 \
})

#define readLastRecord(filename, data) ({   \
    int fd = open(filename,O_RDONLY, 0644); \
    flock(fd,LOCK_SH);                      \
    lseek(fd,-sizeof(data),SEEK_END);       \
    read(fd,&data,sizeof(data));            \
    flock(fd,LOCK_UN);                      \
    close(fd);                              \
})

#define getNextId(filename, recordType, idType, nextId) ({  \
    recordType temp;                        \
    readLastRecord(filename,temp);          \
    nextId = temp.idType + 1;               \
})

long getRecSize(char *filename){
    struct stat fileStat;

    if (stat(filename, &fileStat) == -1) 
        return -1;  
        
    return fileStat.st_size;
}

#endif