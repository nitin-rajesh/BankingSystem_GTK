#include"network/client_tools.h"
#include"commons/crud.h"

#ifndef SERIALIZER
#define SERIALIZER


int sendDataBlock(int sockfd, DataBlock* block){

    size_t total_size = sizeof(FunctionType) + sizeof(int) + sizeof(double) + sizeof(size_t) + block->plSize;

    // Allocate memory to hold the serialized data
    void *buffer = malloc(total_size);
    if (!buffer) {
        perror("Memory allocation failed");
        return -1;
    }

    // Serialize DataBlock into buffer
    size_t offset = 0;

    memcpy(buffer + offset, &block->crudOp, sizeof(FunctionType));
    offset += sizeof(FunctionType);

    memcpy(buffer + offset, &block->id, sizeof(int));
    offset += sizeof(int);

    memcpy(buffer + offset, &block->amount, sizeof(double));
    offset += sizeof(double);

    memcpy(buffer + offset, &block->plSize, sizeof(size_t));
    offset += sizeof(size_t);

    // Copy the payload (if any)
    if (block->plSize > 0 && block->payload) {
        memcpy(buffer + offset, block->payload, block->plSize);
    }

    offset += block->plSize;

    // Send the serialized data
    if (send(sockfd, buffer, total_size, 0) < 0) {
        perror("Send failed");
        free(buffer);
        return -1;
    }

    printf("Sent %ld bytes of data\n",offset);
    free(buffer);
    return 0;
}


long receiveDataBlock(int sockfd, DataBlock* block) {
    // Receive the metadata first (without payload)
    size_t recv_size = sizeof(FunctionType) + sizeof(int) + sizeof(double) + sizeof(size_t);

    void *buffer = malloc(recv_size);
    if (!buffer) {
        perror("Memory allocation failed");
        return -1;
    }

    if (recv(sockfd, buffer, recv_size, 0) <= 0) {
        perror("Failed to receive metadata");
        free(buffer);
        return -1;
    }

    // Deserialize metadata into the DataBlock
    size_t offset = 0;

    memcpy(&block->crudOp, buffer + offset, sizeof(FunctionType));
    offset += sizeof(FunctionType);

    memcpy(&block->id, buffer + offset, sizeof(int));
    offset += sizeof(int);

    memcpy(&block->amount, buffer + offset, sizeof(double));
    offset += sizeof(double);

    memcpy(&block->plSize, buffer + offset, sizeof(size_t));
    offset += sizeof(size_t);

    free(buffer);  // Free temporary buffer

    // Allocate memory for the payload if it exists
    if (block->plSize > 0) {
        block->payload = malloc(block->plSize);
        if (!block->payload) {
            perror("Payload memory allocation failed");
            return -1;
        }

        // Receive the payload
        if (recv(sockfd, block->payload, block->plSize, 0) <= 0) {
            perror("Failed to receive payload");
            free(block->payload);
            return -1;
        }
    }

    offset += block->plSize;

    printf("Received DataBlock: crudOp=%d, id=%d, amount=%.2f \npayload:%ld bytes\n",
           block->crudOp, block->id, block->amount, block->plSize);

    return offset;
}


#endif