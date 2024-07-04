#include <sys/wait.h>
#include "Buffer.h"

void produceCycle(Buffer * ring) {
    while (1) {
        Data data = initData();
        pushWithSemaphores(ring, data);
        if (ring->tail < MAX_BUFF_COUNT - 1) {
            printf("created message\n");
            dataOutput(data);
        }
        sleep(8);
    }
}
void consumeCycle(Buffer* ring) {
    while (1) {
        Data data = popWithSemaphores(ring);
        printf("taken message\n");
        dataOutput(data);
        sleep(4);
    }
}

void addProducer(Buffer * buf, ChildrenPid* cp, int ringKey, int semaphoresKey) {

    if(cp->producersCount < MAX_BUFF_COUNT) {
        pid_t pid_child = fork();

        if (pid_child == 0) {
            buf = (Buffer *) shmat(ringKey, NULL, 0);
            if (buf == (void *) -1) {
                perror("error\n");
                exit(EXIT_FAILURE);
            }
            buf->semaphores = (Semaphores *) shmat(semaphoresKey, NULL, 0);
            if (buf->semaphores == (void *) -1) {
                perror("error\n");
                exit(EXIT_FAILURE);
            }

            produceCycle(buf);

        } else if (pid_child == -1) {
            printf("Unable to create a daughter process\n");
        } else {
            cp->producersCount++;
            cp->producerPids[cp->producersCount - 1] = pid_child;
        }
    } else {
        printf("producersCount is MAX\n");
    }
}

void addConsumer(Buffer* buf, ChildrenPid* cp, int ringKey, int semaphoresKey) {
    if(cp->consumersCount < MAX_BUFF_COUNT) {
        pid_t pid_child = fork();

        if (pid_child == 0) {

            buf = (Buffer *) shmat(ringKey, NULL, 0);
            if (buf == (void *) -1) {
                perror("void addConsumer() error\n");
                exit(EXIT_FAILURE);
            }
            buf->semaphores = (Semaphores *) shmat(semaphoresKey, NULL, 0);
            if (buf->semaphores == (void *) -1) {
                perror("void addProducer(ConcurrentRing* r) error\n");
                exit(EXIT_FAILURE);
            }

            consumeCycle(buf);

        } else if (pid_child == -1) {
            printf("[void addConsumer()]\tUnable to create a daughter process\n");
        } else {
            cp->consumersCount++;
            cp->consumerPids[cp->consumersCount - 1] = pid_child;
        }
    } else {
        printf("MAX consumersCount\n");
    }
}

void killProducer(Buffer* buf, ChildrenPid* cp) {
    if(cp->producersCount != 0) {
        int status;
        int child_pid = cp->producerPids[cp->producersCount - 1];
        int child_pidCopy = child_pid;
        kill(child_pid, SIGTERM);
        waitpid(child_pid, &status, 0);
        cp->producersCount--;
        if ((status)) {
            printf("INFO\n");
            printf("Last child with PID = %d popped. %d left\n", child_pidCopy, cp->producersCount);
        }
    } else {
        printf("producersCount is 0\n");
    }
}

void killConsumer(Buffer* buf, ChildrenPid* cp) {

    if(cp->consumersCount != 0) {
        int status;
        int child_pid = cp->consumerPids[cp->consumersCount - 1];
        int child_pidCopy = child_pid;
        kill(child_pid, SIGTERM);
        waitpid(child_pid, &status, 0);
        cp->consumersCount--;
        if (WIFSIGNALED(status)) {
            printf("INFO\n");
            printf("Last child with PID = %d popped. %d left\n", child_pidCopy, cp->consumersCount);
        }
    } else {
        printf("consumersCount is 0\n");
    }
}

void showStats(Buffer* buf) {
    printf("stats:\n"
           "head: %d,\ttail: %d,\taddedCount: %d,\textractedCount: %d\n",
           buf->head, buf->tail, buf->addedCount, buf->deletedCount);
}


void chooseOptionCycle(Buffer* buf, ChildrenPid* cp, int ringKey, int semaphoresKey) {
    char option;
    printf(">\t'1' Add producer\n>\t'2' Add consumer\n>\t'3' Kill producer\n>\t'4' Kill consumer\n>\t'5' Show stats\n>\t'q' Quit\n");

    while(1) {

        option = getchar();
        getchar();

        switch(option) {
            case '1':
                addProducer(buf, cp, ringKey, semaphoresKey);
                break;
            case '2':
                addConsumer(buf, cp, ringKey, semaphoresKey);
                break;
            case '3':
                killProducer(buf, cp);
                break;
            case '4':
                killConsumer(buf, cp);
                break;
            case '5':
                showStats(buf);
                break;
            case 'q':
                exit(-1);
            default:
                printf("invalid input\n");
        }
    }
}


int main() {
    int ringKey;
    int semaphoresKey;

    Buffer* buf = initBuf(&ringKey, &semaphoresKey);
    ChildrenPid* cp = initChildPid();

    chooseOptionCycle(buf, cp, ringKey, semaphoresKey);
}

