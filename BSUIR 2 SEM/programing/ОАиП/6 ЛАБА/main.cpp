#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>//usleep
#include <signal.h>

#define NUM_EMPLOYEES 3 // количество сотрудников


int isSimulationGoing = 1;

int num_arrived = 0; // количество пришедших людей
int num_left = 0; // количество ушедших людей
int num_busy[NUM_EMPLOYEES] = {0}; // количество занятых сотрудников
int total_busy_time[NUM_EMPLOYEES] = {0}; // общее время занятости сотрудников
int sim_time = 0; // время симуляции в секундах
float arrival_chance = 1; // шанс прихода человека в очередь
float departure_chance = 0.1; // шанс ухода человека из очереди
float consultation_chance = 0.3; // шанс начала консультации

void setSimulationParameters()
{
    printf("-------------------------\n");
    printf("Enter arrival chance: ");
    scanf("%f", &arrival_chance);
    printf("Enter departure chance: ");
    scanf("%f", &departure_chance);
    printf("Enter consultation chance: ");
    scanf("%f", &consultation_chance);
    printf("-------------------------\n");
}

void showSimulationStats()
{
    // вывод результатов симуляции
    float avg_queue_size = (float)(num_arrived - num_left) / sim_time;
    printf("Mean queue load: %f\n", avg_queue_size);

    for (int i = 0; i < NUM_EMPLOYEES; i++) {
        float avg_busy_time = (float)total_busy_time[i] / sim_time;
        printf("Mean load of consultant for one person %d: %f sec\n", i+1, avg_busy_time);
    }

    printf("Overall number of persons: %d\n", num_arrived);
    printf("Number of people the left: %d\n", num_left);
}

// Signal handler function
void interrupt_handler(int sig) {
    signal(sig, interrupt_handler);
    isSimulationGoing = 0;
    printf("-------------------------\n");
    showSimulationStats();
    printf("-------------------------\n");
    printf("Press 'Y' if you want to change simulation parameters\n");
    printf("Press 'Q' if you want to quit simulation\n");
    printf("Press any other key if you want to continue\n");
    fflush(stdin);
    char choice = getchar();
    if (choice == 'y' || choice == 'Y')
    {
        setSimulationParameters();
        system("pause");
    }
    if (choice == 'q' || choice == 'Q')
    {
        exit(0);
    }
    isSimulationGoing = 1;
}

// структура для представления человека в очереди
typedef struct {
    int id; // идентификатор человека
    time_t arrival_time; // время прихода в очередь
} Person;

// структура для представления очереди
struct Queue {
    Person queue; // массив людей в очереди
    struct Queue* next; // следующий за текущим человек в очереди
};

// функция для добавления человека в очередь
void enqueue(Queue **qu, Person p) {
    struct Queue* q = (*qu);
    struct Queue* newElement = (struct Queue*)malloc(sizeof(struct Queue));
    newElement->queue = p;
    newElement->next=NULL;
    if (q == NULL)
    {
        q = newElement;
    }
    else
    {
        struct Queue* tmp = q;
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next=newElement;
    }
    *qu = q;
}

// функция для удаления человека из очереди
Person dequeue(Queue **qu) {
    struct Queue* q = (*qu);
    if (q == NULL)
    {
        printf("Queue is empty.\n");
        Person p;
        p.id = -1;
        return p;
    }
    Person result = q->queue;
    q = q->next;
    *qu = q;
    return result;
}

void simulationProcess()
{
    struct Queue* q = NULL;
    while (1) {
        if (isSimulationGoing) {
            // генерация прихода человека в очередь
            if ((float) rand() / RAND_MAX < arrival_chance) {
                Person p;
                p.id = ++num_arrived;
                p.arrival_time = sim_time;
                enqueue(&q, p);
                printf("Person %d joined the queue on %d second.\n", p.id, p.arrival_time);
            }

            // генерация ухода человека из очереди
            if ((float) rand() / RAND_MAX < departure_chance) {
                Person p = dequeue(&q);
                if (p.id != -1) {
                    num_left++;
                    printf("Person %d quit the queue on %d second.\n", p.id, sim_time);
                }
            }

            // генерация начала консультации
            for (int i = 0; i < NUM_EMPLOYEES; i++) {
                if (num_busy[i] == 0 && q != NULL && (float) rand() / RAND_MAX < consultation_chance) {//макс число для инта
                    Person p = dequeue(&q);
                    num_busy[i] = 1;
                    total_busy_time[i] += sim_time - p.arrival_time;
                    printf("Consultant %d started consultation with person %d on %d second.\n", i + 1, p.id, sim_time);
                }
            }

            // проверка окончания консультации
            for (int i = 0; i < NUM_EMPLOYEES; i++) {
                if (num_busy[i] == 1 && (float) rand() / RAND_MAX < consultation_chance) {
                    num_busy[i] = 0;
                    printf("Consultant %d finished the consultation on %d second.\n", i + 1, sim_time);
                }
            }

            sim_time++;
            usleep(10000); // sleep for 0.01
        }
    }
}

int main() {
    signal(SIGBREAK, interrupt_handler);
    srand(time(NULL)); // инициализация генератора случайных чисел
    setSimulationParameters();
    simulationProcess();
    system("pause");
    return 0;
}
//fn 5
//inter... прерывание
// handler обработчик