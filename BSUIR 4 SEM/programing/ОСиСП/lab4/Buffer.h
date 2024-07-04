#pragma once
#ifndef LAB4_BUFFER_H
#define LAB4_BUFFER_H

#include <stdio.h>
#include <stdint-gcc.h>
#include <unistd.h>
#include <signal.h>
#include <malloc.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX_BUFF_COUNT 10

typedef struct {
    uint8_t type;
    unsigned short hash;
    uint8_t size;
    uint8_t data[256];
} Data;

typedef struct {
    sem_t mutex;
    sem_t free_space;
    sem_t items;
} Semaphores;

typedef struct {
    Semaphores* semaphores;

    int nodesCountForNow;
    int addedCount;
    int deletedCount;

    int head;
    int tail;
    Data dates[MAX_BUFF_COUNT];

} Buffer;

typedef struct {
    int producersCount;
    int consumersCount;

    int producerPids[MAX_BUFF_COUNT];
    int consumerPids[MAX_BUFF_COUNT];
} ChildrenPid;


Buffer* initBuf(int* ringKey, int* semaphoresKey);
ChildrenPid* initChildPid();

void initSemaphores(Semaphores* semaphores);
int initSharedMemory(int number);
Data initData();

void pushWithSemaphores(Buffer *ring, Data data);
Data popWithSemaphores(Buffer *ring);

void dataOutput(Data data);


#endif //LAB4_BUFFER_H
