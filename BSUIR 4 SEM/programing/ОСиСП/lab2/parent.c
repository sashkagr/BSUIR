#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

#include <errno.h>
#include <ctype.h>

#include <sys/wait.h>

enum parent {
    parent_TERMINATE,
    parent_TRIGGER_PRINT,
    parent_REPLY,//разрешает выводить информацию

};

enum child {
    child_QUERY,//запрос на вывод
    child_INFORM,//выводит статистику
};

#define MAX_childS 100
#define TIMER_DURATION_SEC 5

typedef struct {
    pid_t pid;
    bool print_allowed;
} child_t;


size_t child_count = 0;
child_t child_array[MAX_childS];

void send_signal(child_t child, enum parent signal) {
    union sigval value = {signal};
    if (sigqueue(child.pid, SIGUSR1, value)) {
        perror("sigqueue");
        exit(errno);
    }
}

size_t get_index_by_pid(pid_t pid) {
    for (size_t i = 0; i < child_count; ++i) {
        if (child_array[i].pid == pid) {
            return i;
        }
    }
    fprintf(stderr, "pid=%d is not a child of the process\n", pid);
    exit(EXIT_FAILURE);
}
pid_t destroy_last_child() {
    if (child_count == 0) {
        fputs("child_array is empty", stderr);
        exit(EXIT_FAILURE);
    }
    send_signal(child_array[--child_count], parent_TERMINATE);
    return wait(NULL);
}
void terminate_all(void) {
    while (child_count > 0) {
        destroy_last_child();
    }
}

void exit_handler(void) {
    terminate_all();
    puts("All child processes are terminated, program is finished");
}


void fork_child(void) {
    if (child_count == MAX_childS - 1) {
        fputs("child count overflow", stderr);
        exit(EXIT_FAILURE);
    }

    pid_t ret = fork();
    switch (ret) {
        case 0: {
            char child_name[9];
            sprintf(child_name, "S_%zu", child_count);
            if (execl("./child", child_name, NULL) == -1) {
                perror("execl");
                exit(errno);
            }
        }
            break;

        case -1:
            perror("fork");
            exit(errno);

        default:
            child_array[child_count].pid           = ret;
            child_array[child_count].print_allowed = true;
            ++child_count;
            break;
    }
}



void halt_statistics(child_t* child) {
    child->print_allowed = false;
}

void resume_statistics(child_t* child) {
    child->print_allowed = true;
}

void display_statistics(child_t child) {
    send_signal(child, parent_TRIGGER_PRINT);
}



void halt_all_statistics(int sig) {
    for (size_t i = 0; i < child_count; ++i) {
        halt_statistics(&child_array[i]);
    }
}

void resume_all_statistics(int sig) {
    for (size_t i = 0; i < child_count; ++i) {
        resume_statistics(&child_array[i]);
    }
}

void child_signal_handler(int sig, siginfo_t* info, void* context) {
    size_t i = get_index_by_pid(info->si_pid);

    switch (info->si_int) {
        case child_QUERY:
            if (child_array[i].print_allowed) {
                send_signal(child_array[i], parent_REPLY);
            }
            break;

        case child_INFORM:
            printf("%d informed parent\n", child_array[i].pid);
            break;

        default:
            fprintf(stderr,
                    "Unknown signal occurred in child_signal_handler: %d\n",
                    info->si_int);
            exit(EXIT_FAILURE);
    }
}



void initialize(void)  {
    if (atexit(exit_handler)) {
        fputs("atexit failed", stderr);
        exit(EXIT_FAILURE);
    }

    if (signal(SIGALRM, resume_all_statistics) == SIG_ERR) {
        perror("signal");
        exit(errno);
    }

    struct sigaction child_signals;
    child_signals.sa_flags     = SA_SIGINFO;
    child_signals.sa_sigaction = child_signal_handler;

    if (sigaction(SIGUSR2, &child_signals, NULL)) {
        perror("sigaction");
        exit(errno);
    }
}



void execute_appropriate_action(void (* single)(child_t*), void (* all)(int)) {
    int inputChar = getc(stdin);
    bool isNumber = isdigit(inputChar);
    ungetc(inputChar, stdin);
    if(isNumber){
        size_t num;
        if (scanf("%zu", &num) != 1) {
            fputs("Error reading from input", stderr);
            exit(EXIT_FAILURE);
        }
        if (num >= child_count) {
            fputs("Num is greater than child_array count", stderr);
        }
        else{
            single(&child_array[num]);
        }

    } else {
        all(1);
    }
}

void p(void) {
    int inputChar = getc(stdin);
    bool isNumber = isdigit(inputChar);
    ungetc(inputChar, stdin);
    if(isNumber) {
        size_t num;
        if (scanf("%zu", &num) != 1) {
            fputs("Error reading from input", stderr);
            exit(EXIT_FAILURE);
        }
        halt_all_statistics(1);
        display_statistics(child_array[num]);
        alarm(TIMER_DURATION_SEC);
    }
    else{
        fputs("Error reading from input", stderr);
        exit(EXIT_FAILURE);
    }
}


int main(void) {
    initialize();

    printf("[+] - spawn child process\n"
           "[-] - terminate last child process\n"
           "[k] - terminate all child processes\n"
           "[l] - show all processes\n"
           "[s] <num> - halt statistics\n"
           "[g] <num> - resume statistics\n"
           "[p] num - display S_{num} statistics and pause other childs\n"
           "[q] - exit\n"
           ">");
    while (1) {
        switch (getchar()) {
            case '+':
                fork_child();
                printf("child with pid=%d was spawned\n", child_array[child_count - 1].pid);
                break;

            case '-':
                destroy_last_child();
                printf("Last child was terminated, %zu left\n", child_count);
                break;

            case 'k':
                terminate_all();
                puts("All child processes were terminated");
                break;
            case 'l':
                for(size_t i=0;i<child_count;i++){
                    send_signal(child_array[i],parent_TRIGGER_PRINT);
                }
                puts("All child processes were show");
                break;

            case 's':
                execute_appropriate_action(halt_statistics, halt_all_statistics);
                break;

            case 'g':
                execute_appropriate_action(resume_statistics, resume_all_statistics);
                break;

            case 'p':
                p();
                break;

            case 'q':
                exit(EXIT_SUCCESS);

            default:
                break;
        }
    }
}
