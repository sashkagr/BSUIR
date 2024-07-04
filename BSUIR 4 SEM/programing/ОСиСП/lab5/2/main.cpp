#include "Ring.hpp"

ConcurrentRing* globalRing;
ChildThreads* globalChilds;


void addProducer();
void addConsumer();
void killProducer();
void killConsumer();
void showStats();
void* produceCycle(void*);
void* consumeCycle(void*);
void chooseOptionCycle();
void IncreaseRing();
void DecreaseRing();
void KillAllChilds();
void help();
void signalHandler(int signum);
int main() {
    signal(SIGINT, signalHandler);
    globalRing = initRing();
    globalChilds = initChilds();
    chooseOptionCycle();
}


void chooseOptionCycle() {
    char option;
    help();
    while(1) {
        std::cout << "\n:>";
        std::cin >> option;

        switch(option) {
            case '1':
                addProducer();
                break;
            case '2':
                addConsumer();
                break;
            case '3':
                killProducer();
                break;
            case '4':
                killConsumer();
                break;
            case '5':
                showStats();
                break;
            case '6':
                IncreaseRing();
                break;
            case '7':
                DecreaseRing();
                break;
            case 'h':
                help();
                break;
            case 'c':
                system("clear");
                help();
                break;
            case 'q':
                KillAllChilds();
                exit(EXIT_SUCCESS);
            default:
                std::cout << "Invalid input\n";
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
}

void increaseSize() {
    Data* newDates = (Data*)malloc(sizeof(Data) * (globalRing->size + 1));
    for (int i = 0; i < globalRing->size; i++) {
        newDates[i] = globalRing->dates[i];
    }
    free(globalRing->dates);
    globalRing->size++;
    globalRing->dates = newDates;
}

void decreaseSize() {
    if (globalRing->size <= 1) {
        return;
    }
    Data* newDates = (Data*)malloc(sizeof(Data) * (globalRing->size - 1));
    for (int i = 0; i < globalRing->size - 1; i++) {
        newDates[i] = globalRing->dates[i];
    }
    free(globalRing->dates);
    globalRing->dates = newDates;
    globalRing->size--;
}

void IncreaseRing() {
    pthread_mutex_lock(&globalRing->mutex);
    increaseSize();
    pthread_cond_signal(&globalRing->cond_empty);
    pthread_mutex_unlock(&globalRing->mutex);
}

void DecreaseRing(){
    pthread_mutex_lock(&globalRing->mutex);
    decreaseSize();
    if(globalRing->tail >= globalRing->size - 1){
        pthread_cond_signal(&globalRing->cond_full);
    }
    pthread_mutex_unlock(&globalRing->mutex);
}

void addProducer() {
    globalChilds->producersCount++;
    pthread_t* updatedProducersIDS = (pthread_t*)malloc(sizeof(pthread_t) * (globalChilds->producersCount));
    for (int i = 0; i < globalChilds->producersCount - 1; i++) {
        updatedProducersIDS[i] = globalChilds->producerIDS[i];
    }
    if (globalRing->tail == -1) {
        pthread_cond_signal(&globalRing->cond_empty);
    }

    if (globalChilds->producerIDS) {
        free(globalChilds->producerIDS);
    }
    globalChilds->producerIDS = updatedProducersIDS;

    pthread_create(&globalChilds->producerIDS[globalChilds->producersCount - 1], NULL, produceCycle, NULL);
}

void addConsumer() {
    globalChilds->consumersCount++;
    pthread_t* updatedConsumersIDS = (pthread_t*)malloc(sizeof(pthread_t) * globalChilds->consumersCount);
    for (int i = 0; i < globalChilds->consumersCount - 1; i++) {
        updatedConsumersIDS[i] = globalChilds->consumerIDS[i];
    }
    if (globalRing->tail == globalRing->size - 1) {
        pthread_cond_signal(&globalRing->cond_full);
    }
    if (globalChilds->consumerIDS) {
        free(globalChilds->consumerIDS);
    }
    globalChilds->consumerIDS = updatedConsumersIDS;

    pthread_create(&globalChilds->consumerIDS[globalChilds->consumersCount - 1], NULL, consumeCycle, NULL);
}

void killProducer() {
    if(globalChilds->producersCount != 0) {
        globalChilds->producersCount--;
        pthread_cancel(globalChilds->producerIDS[globalChilds->producersCount]);

        if (globalChilds->producersCount > 0) {
            pthread_t* updatedProducersIDS = (pthread_t*)malloc(sizeof(pthread_t) * (globalChilds->producersCount));
            for (int i = 0; i < globalChilds->producersCount; i++) {
                updatedProducersIDS[i] = globalChilds->producerIDS[i];
            }
            free(globalChilds->producerIDS);
            globalChilds->producerIDS = updatedProducersIDS;
        } else {
            free(globalChilds->producerIDS);
            globalChilds->producerIDS = NULL;
        }
        std::cout << "\nLast producer popped. " << globalChilds->producersCount << " left\n";
    } else {
        std::cout << "\nThere is no producers\n";
    }
}

void killConsumer() {
    if (globalChilds->consumersCount != 0) {
        pthread_cancel(globalChilds->consumerIDS[globalChilds->consumersCount - 1]);
        globalChilds->consumersCount--;


        if (globalChilds->consumersCount > 0) {
            pthread_t* updatedConsumersIDS = (pthread_t*)malloc(sizeof(pthread_t) * (globalChilds->consumersCount));
            if (updatedConsumersIDS == NULL) {
                exit(EXIT_FAILURE);
            }
            for (int i = 0; i < globalChilds->consumersCount; i++) {
                updatedConsumersIDS[i] = globalChilds->consumerIDS[i];
            }
            free(globalChilds->consumerIDS);
            globalChilds->consumerIDS = updatedConsumersIDS;
        } else {
            free(globalChilds->consumerIDS);
            globalChilds->consumerIDS = NULL;
        }
        std::cout << "\nLast consumer popped. " << globalChilds->consumersCount << " left\n";
    } else {
        std::cout << "\nThere is no consumers\n";
    }
}

void* produceCycle(void*) {
    while (1) {
        pthread_mutex_lock(&globalRing->mutex);

        while (globalRing->tail >= globalRing->size - 1) {
            std::cout << "Producer waiting\n";
            pthread_cond_wait(&globalRing->cond_empty, &globalRing->mutex);
        }

        Data data = initData();
        push(globalRing, data);
        std::cout << "\n<Created message>. Ring msgs:" << globalRing->tail << "\n";
        printData(data);
        pthread_cond_signal(&globalRing->cond_full);
        pthread_mutex_unlock(&globalRing->mutex);
        sleep(3);
    }
}

void* consumeCycle(void*) {
    while (1) {
        pthread_mutex_lock(&globalRing->mutex);

        while (globalChilds->producersCount > 0 && globalRing->tail == -1) {
            std::cout << "Consumer waiting\n";
            pthread_cond_wait(&globalRing->cond_full, &globalRing->mutex);
        }
        if (globalChilds->producersCount == 0 && globalRing->tail == -1) {
            pthread_mutex_unlock(&globalRing->mutex);
        }
        if (globalRing->tail != -1) {
            Data data = pop(globalRing);
            std::cout << "\n<Taken message>. Ring msgs: " << globalRing->tail << "\n";
            printData(data);

            if(globalRing->tail == -1){
                pthread_cond_signal(&globalRing->cond_empty);
            }
            else{
                pthread_cond_signal(&globalRing->cond_full);
            }
        }
        pthread_mutex_unlock(&globalRing->mutex);
        sleep(2);
    }
}

void KillAllChilds(){
    for (int i = 0; i < globalChilds->consumersCount; i++){
        pthread_cancel(globalChilds->consumerIDS[i]);
    }
    for (int i = 0; i < globalChilds->producersCount; i++){
        pthread_cancel(globalChilds->producerIDS[i]);
    }
}

void signalHandler(int signum) {
    KillAllChilds();
    std::cout << "<ctrl+c> [" << signum << "] Exiting\n";
    exit(signum);
}

void help(){
    std::cout << "\n"
              << "> '1' Add producer\n"
              << "> '2' Add consumer\n"
              << "> '3' Kill producer\n"
              << "> '4' Kill consumer\n"
              << "> '5' Show stats\n"
              << "> '6' Increase ring size\n"
              << "> '7' Decrease ring size\n"
              << "> 'h' For help(menu)\n"
              << "> 'c' For clear screen\n"
              << "> 'q' Quit\n";
}

void showStats() {
    std::cout << "\n<===>STATS<===>\n"
              << "Consumers: " << globalChilds->consumersCount << "\n"
              << "Producers: " << globalChilds->producersCount << "\n"
              << "Ring size: " << globalRing->size << "\n"
              << "Ring Head: " << globalRing->head << "\n"
              << "Ring Tail: " << globalRing->tail << "\n"
              << "AddedCount: " << globalRing->addedCount << "\n"
              << "ExtractedCount: " << globalRing->extractedCount << "\n\n";
}
