//
// Created by sasha on 23.04.24.
//

#ifndef INC_5_RING_HPP
#define INC_5_RING_HPP

#endif //INC_5_RING_HPP
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

typedef struct {
    sem_t mutex;
    sem_t full;
    sem_t empty;
} Semaphores;

typedef struct {
    Semaphores* semaphores;

    int nodesCountForNow;
    int addedCount;
    int extractedCount;

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

void initSemaphores(Semaphores* semaphores);
Data initData();

void push(ConcurrentRing *r, Data data);
Data pop(ConcurrentRing *ring);

void printData(Data data);


#endif //LAB5_CONCURRENTRING_H
