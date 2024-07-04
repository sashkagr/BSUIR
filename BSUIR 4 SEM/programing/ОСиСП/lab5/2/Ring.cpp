#include "Ring.hpp"


ConcurrentRing* initRing() {

    ConcurrentRing* ring = (ConcurrentRing*) malloc(sizeof(ConcurrentRing));
    if (ring == NULL) {
        perror("Memory error for ConcurrentRing\n");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_init(&ring->mutex, NULL);
    pthread_cond_init(&ring->cond_empty, NULL);
    pthread_cond_init(&ring->cond_full, NULL);

    ring->size = INIT_BUFF_COUNT;

    ring->dates = (Data*)malloc(sizeof(Data)*ring->size);

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
    cp->consumerIDS = NULL;
    cp->producerIDS = NULL;
    return cp;
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
}


Data pop(ConcurrentRing *ring) {
    Data data;
    if (ring->tail == -1) {
        return NULL_DATA;
    }

    data = ring->dates[ring->tail];
    ring->tail--;

    if(ring->tail == -1) {
        ring->head--;
    }
    ring->extractedCount++;
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
