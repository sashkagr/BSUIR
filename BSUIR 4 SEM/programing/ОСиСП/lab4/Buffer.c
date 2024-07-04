#include <time.h>
#include "Buffer.h"

Buffer* initBuf(int* ringKey, int* semaphoresKey) {

    (*ringKey) = initSharedMemory(1);
    (*semaphoresKey) = initSharedMemory(2);

    Buffer* buf = (Buffer*) shmat((*ringKey), NULL, 0);
    if (buf == (void*)-1) {
        perror("error\n");
        exit(EXIT_FAILURE);
    }
    Semaphores* semaphores = (Semaphores*) shmat((*semaphoresKey), NULL, 0);
    if (semaphores == (void*)-1) {
        perror("error\n");
        exit(EXIT_FAILURE);
    }

    initSemaphores(semaphores);
    buf->semaphores = semaphores;

    buf->head = -1;
    buf->tail = -1;

    buf->deletedCount = 0;
    buf->addedCount = 0;

    return buf;
}

ChildrenPid* initChildPid() {
    ChildrenPid* cp = malloc(sizeof(ChildrenPid));
    cp->consumersCount = 0;
    cp->producersCount = 0;

    return cp;
}
void initSemaphores(Semaphores* semaphores) {
    if (sem_init(&(semaphores->mutex), 1, 1) == -1) {
        perror("initSemaphores(mutex)");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&(semaphores->free_space), 1, 0) == -1) {
        perror("initSemaphores(full)");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&(semaphores->items), 1, MAX_BUFF_COUNT - 1) == -1) {
        perror("initSemaphores(empty)");
        exit(EXIT_FAILURE);
    }
}

int initSharedMemory(int number) {

    key_t ipcKey = ftok(".", number);

    if (ipcKey == -1) {
        perror("fail ftok");
        exit(EXIT_FAILURE);
    }

    int sharedMemoryId;
    if (number == 1) {
        sharedMemoryId = shmget(ipcKey, sizeof(Buffer), IPC_CREAT | 0666);
    } else {
        sharedMemoryId = shmget(ipcKey, sizeof(Semaphores), IPC_CREAT | 0666);
    }

    if (sharedMemoryId == -1) {
        perror("int initSharedMemory(int proj_id) fail shmget");
        exit(EXIT_FAILURE);
    }

    return sharedMemoryId;
}

Data initData() {
    srand(time(NULL));

    Data data;
    data.hash = 0;
    short size;
    do {
        size = rand() % 257;
    } while (size == 0);

    for (int i = 0; i < size; i++) {
        data.data[i] = rand() % 256;
        data.hash += data.data[i] ^ data.hash;
    }
    if (size == 256) {
        data.size = 0;
    } else {
        data.size = size;
    }

    return data;
}

void pushWithSemaphores(Buffer *buf, Data data) {
    sem_wait(&(buf->semaphores->items));
    sem_wait(&(buf->semaphores->mutex));

    if (buf->tail < MAX_BUFF_COUNT - 1) {

        if (buf->tail == -1) {
            buf->head++;
        }

        buf->tail++;
        buf->dates[buf->tail] = data;
        buf->addedCount++;

    } else {
        printf("[ConcurrentRing] can not add new message. Ring is full\n");
    }
    sem_post(&(buf->semaphores->mutex));
    sem_post(&(buf->semaphores->free_space));
}

Data popWithSemaphores(Buffer *buf) {
    sem_wait(&(buf->semaphores->free_space)); //-1
    sem_wait(&(buf->semaphores->mutex));

    Data data;
    data = buf->dates[buf->tail];

    buf->tail--;

    if(buf->tail == -1) {
        buf->head--;
    }
    buf->deletedCount++;

    sem_post(&(buf->semaphores->mutex));
    sem_post(&(buf->semaphores->items));
    return data;
}

void dataOutput(Data data) {
    printf("type: %u, hash: %d, size: %u, data: ", data.type, data.hash, data.size);
    short size = data.size;
    if(size == 0) {
        size = 256;
    }

    for(short i = 0; i < size; i++) {
        printf("%u ", data.data[i]);
    }
    printf("\n");

}
