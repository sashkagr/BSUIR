#include <stdbool.h>
#include <stddef.h>
#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <unistd.h>

#define PRINT_LIMIT 101
#define ALARM_INTERVAL 10000

enum parent {
    parent_TERMINATE,
    parent_TRIGGER_PRINT,
    parent_REPLY,
};

enum child {
    child_QUERY,
    child_INFORM,
};


typedef struct {
    int O;
    int I;
} binary_t;

typedef struct {
    size_t O_O;
    size_t O_I;
    size_t I_O;
    size_t I_I;
} statistics_t;

static const binary_t zero = {0, 0};
static const binary_t three = {1, 1};
binary_t data;
statistics_t stats;
int alarm_counter;
bool allow_output;


void display_statistics(void) {
    char buffer[256];
    char* ptr = buffer;
    sprintf(buffer, "ppid=%d, pid=%d, stats={%zu, %zu, %zu, %zu}\n",
            getppid(), getpid(), stats.O_O, stats.O_I, stats.I_O, stats.I_I);
    while (*ptr) {
        putchar(*ptr);
        ++ptr;
    }
}

void parent_signal_handler(int sig, siginfo_t* info, void* context) {
    switch (info->si_int) {
        case parent_REPLY:
            allow_output = true;
            break;

        case parent_TRIGGER_PRINT:
            display_statistics();
            break;

        case parent_TERMINATE:
            exit(EXIT_SUCCESS);

        default:
            fprintf(stderr,
                    "Unknown signal occurred: %d\n",
                    info->si_int);
            exit(EXIT_FAILURE);
    }
}


void exit_handler(void) {
    printf("Process with pid=%d has ended\n", getpid());
}

void alarm_signal_handler(int sig) {
    if (data.I == 0) {
        if (data.O == 0) {
            ++stats.O_O;
        } else {
            ++stats.O_I;
        }
    } else {
        if (data.O == 0) {
            ++stats.I_O;
        } else {
            ++stats.I_I;
        }
    }
    ++alarm_counter;
}


void initialize_timer(struct itimerval* timer) {
    if (atexit(exit_handler)) {
        fputs("atexit failed", stderr);
        exit(EXIT_FAILURE);
    }

    struct sigaction parent_signals;
    parent_signals.sa_flags = SA_SIGINFO;
    parent_signals.sa_sigaction = parent_signal_handler;

    if (sigaction(SIGUSR1, &parent_signals, NULL)) {
        perror("sigaction");
        exit(errno);
    }

    if (signal(SIGALRM, alarm_signal_handler) == SIG_ERR) {
        perror("signal");
        exit(errno);
    }

    timer->it_value.tv_sec = 0;
    timer->it_value.tv_usec = ALARM_INTERVAL;
    timer->it_interval.tv_sec = 0;
    timer->it_interval.tv_usec = ALARM_INTERVAL;

    if (setitimer(ITIMER_REAL, timer, NULL)) {
        perror("setitimer");
        exit(errno);
    }
}


int main(void) {
    struct itimerval timer;
    initialize_timer(&timer);

    while (1) {
      data = zero;
      data = three;

        if (alarm_counter == PRINT_LIMIT) {
            union sigval value = {child_QUERY};
            if (sigqueue(getppid(), SIGUSR2, value)) {
                perror("sigqueue");
                exit(errno);
            }
            if (allow_output) {
                display_statistics();
                union sigval value = {child_INFORM};
                if (sigqueue(getppid(), SIGUSR2, value)) {
                    perror("sigqueue");
                    exit(errno);
                }
            }
            alarm_counter = 0;
            allow_output = false;
            stats.O_O = 0;
            stats.O_I = 0;
            stats.I_O = 0;
            stats.I_I = 0;
        }
    }
}

