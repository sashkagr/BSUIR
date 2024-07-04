#pragma once
#ifndef LAB5_CONCURRENTRING_H
#define LAB5_CONCURRENTRING_H

#include <stdio.h>
#include "iostream"
#include "limits"
#include <stdint-gcc.h>
#include <unistd.h>
#include <signal.h>
#include <malloc.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

#define INIT_BUFF_COUNT 5

typedef struct {
    uint8_t type;
    unsigned short hash;
    uint8_t size;
    uint8_t data[256];
} Data;

#define NULL_DATA {9, 0, 0, {0}}

typedef struct {
    int nodesCountForNow;
    int addedCount;
    int extractedCount;

    pthread_mutex_t mutex;
    pthread_cond_t cond_empty;
    pthread_cond_t cond_full;

    int head;
    int tail;
    int size;
    Data* dates;

} ConcurrentRing;

typedef struct {
    int producersCount;
    int consumersCount;

    pthread_t* producerIDS;
    pthread_t* consumerIDS;
} ChildThreads;

ConcurrentRing* initRing();
ChildThreads* initChilds();

Data initData();

void push(ConcurrentRing *r, Data data);
Data pop(ConcurrentRing *ring);

void printData(Data data);


#endif //LAB5_CONCURRENTRING_H

#ifndef INC_5_2_RING_HPP
#define INC_5_2_RING_HPP

#endif //INC_5_2_RING_HPP
