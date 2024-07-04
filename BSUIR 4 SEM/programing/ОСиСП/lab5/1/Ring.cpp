#include "Ring.hpp"

ConcurrentRing* initRing() {

    ConcurrentRing* ring = (ConcurrentRing*) malloc(sizeof(ConcurrentRing));
    if (ring == NULL) {
        perror("Memory error for ConcurrentRing\n");
        exit(-1);
    }

    Semaphores* semaphores = (Semaphores*) malloc(sizeof(Semaphores));
    if (semaphores == NULL) {
        perror("Memory error for Semaphores\n");
        exit(-1);
    }

    initSemaphores(semaphores);
    ring->semaphores = semaphores;
    ring->size = INIT_BUFF_COUNT;
    ring->dates = (Data*)malloc(sizeof(Data)*(ring->size));
    ring->head = -1;
    ring->tail = -1;

    ring->extractedCount = 0;
    ring->addedCount = 0;

    return ring;
}

ChildThreads* initChilds() {
    ChildThreads* cp = (ChildThreads*)malloc(sizeof(ChildThreads));
    cp->consumersCount = 0;
    cp->producersCount = 0;

    return cp;
}

void initSemaphores(Semaphores* semaphores) {
    if (sem_init(&(semaphores->mutex), 1, 1) == -1) {
        perror("initSemaphores(mutex)");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&(semaphores->full), 1, 0) == -1) {
        perror("initSemaphores(full)");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&(semaphores->empty), 1, INIT_BUFF_COUNT - 1) == -1) {
        perror("initSemaphores(empty)");
        exit(EXIT_FAILURE);
    }
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
        data.hash += data.data[i] * i;
    }
    if (size == 256) {
        data.size = 0;
    } else {
        data.size = size;
    }

    return data;
}

void push(ConcurrentRing *ring, Data data) {
    int empty_value;
    int mutex_value;

    if (sem_trywait(&(ring->semaphores->empty)) == 0) {
        empty_value = 0;
    } else {
        empty_value = -1;
    }

    if (sem_trywait(&(ring->semaphores->mutex)) == 0) {
        mutex_value = 0;
    } else {
        mutex_value = -1;
    }

    if (empty_value == 0 && mutex_value == 0) {
        if (ring->tail < ring->size) {

            if (ring->tail == -1) {
                ring->head++;
            }

            ring->tail++;
            ring->dates[ring->tail] = data;
            ring->addedCount++;
        } else {
            std::cout << "\nRing is full\n";
        }
        sem_post(&(ring->semaphores->full));
    }
    if(mutex_value == 0){
        sem_post(&(ring->semaphores->mutex));
    }
}


Data pop(ConcurrentRing *ring) {
    sem_wait(&(ring->semaphores->full));
    sem_wait(&(ring->semaphores->mutex));

    Data data;
    data = ring->dates[ring->tail];

    ring->tail--;

    if(ring->tail == -1) {
        ring->head--;
    }
    ring->extractedCount++;

    sem_post(&(ring->semaphores->mutex));
    sem_post(&(ring->semaphores->empty));
    return data;
}

void printData(Data data) {
    std::cout << "type: " << std::dec << static_cast<unsigned int>(data.type) << "\n"
              << "hash: " << data.hash << "\n"
              << "size: " << std::dec << static_cast<unsigned int>(data.size) << "\n"
              << "data: ";
    short size = data.size;
    if(size == 0) {
        size = 256;
    }
    for(short i = 0; i < size; i++) {
        std::cout << std::dec << static_cast<unsigned int>(data.data[i]) << " ";
    }
    std::cout << "\n";

}

