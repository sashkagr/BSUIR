#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int globalId = 0;
struct Note
{
    int id;
    char name[50];
    time_t date;
    struct description
    {
        char place[50];
        char members[50];
        char goals[250];

    }description;

};
struct Node {
    struct Note data;
    struct Node* prev;
    struct Node* next;
};
struct Node* head = NULL;
struct Node* tail = NULL;
enum commands
{
    show = 1,
    add,
    dremove,
    update,
    sort,
    dweek,
    day,
    file,
    list
};
enum parameter {
        name=1,
        place,
        members,
        goals,
        date
};

struct Note setNote() {
    printf("%d\n", globalId);
    printf("Enter structure fields\n");
    struct Note note;
    printf("Enter structure name: ");
    fflush(stdin);
    scanf("%[^\n]", note.name);
    printf("Enter structure place: ");
    fflush(stdin);
    scanf("%[^\n]", note.description.place);
    printf("Enter structure members: ");
    fflush(stdin);
    scanf("%[^\n]", note.description.members);
    printf("Enter structure goals: ");
    fflush(stdin);
    scanf("%[^\n]", note.description.goals);
    fflush(stdin);
    struct tm event_time;
    char input_str[100];
    int correct=1;
    while(correct!=0){
        printf("Enter the date and time of the event in the format dd.mm.yyyy hh:mm:ss: ");
        fgets(input_str, sizeof(input_str), stdin);
        sscanf(input_str, "%d.%d.%d %d:%d:%d\n", &event_time.tm_mday, &event_time.tm_mon, &event_time.tm_year,
               &event_time.tm_hour, &event_time.tm_min, &event_time.tm_sec);
        event_time.tm_mon -= 1; event_time.tm_year -= 1900; note.date =mktime(&event_time);
        time_t now;
        time(&now);
        struct tm *local = localtime(&now);
        time_t nowDate=mktime(local);
        if((mktime(&event_time)==-1)||(nowDate>note.date)) {
            printf("Error! Enter correct date: ");}
        else correct=0;
    }
    return note;
}
void printNote(struct Note note){
    printf("%d  |   ", note.id);
    printf("%8s  |   ", note.name);
    printf("%12s  |   ", note.description.place);
    printf("%12s  |   ", note.description.members);
    printf("%20s  |   ", note.description.goals);
    tm* aTm = localtime(&note.date);
    printf("%02d/%02d/%04d %02d:%02d:%02d   |", aTm->tm_mday, aTm->tm_mon+1,aTm->tm_year+1900, aTm->tm_hour, aTm->tm_min, aTm->tm_sec);

}
void insertAtEnd() {
    struct Note new_data=setNote();
    new_data.id = globalId;
    globalId++;

    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    struct Node* last = head;
    new_node->data = new_data;
    new_node->next = NULL;
    if (head == NULL) {
        new_node->prev = NULL;
        head = new_node;
        tail= new_node;
        return;
    }
    while (last->next != NULL) {
        last = last->next;
    }
    last->next = new_node;
    new_node->prev = last;
    tail=new_node;
    return;
}
void addToList(FILE* rp) {
    FILE *fp = fopen("notes.bin", "rb");
    struct Note new_data;
    int count=0;
      if (fp != NULL) {
          while (fread(&new_data, sizeof(struct Note), 1, fp) == 1) {
              new_data.id = globalId;
              globalId++;
              struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
              struct Node* last = head;
              new_node->data = new_data;
              new_node->next = NULL;
              if (head == NULL) {
                  new_node->prev = NULL;
                  head = new_node;
                  tail= new_node;
              }
              else{
              while (last->next != NULL) {
                  last = last->next;
              }
              last->next = new_node;
              new_node->prev = last;
              tail=new_node;
              count++;
         }}
        fclose(fp);
          time_t now;
          time(&now);
          struct tm *local = localtime(&now);
          if(count==0){
              fprintf(rp, "[Add to list from file]:[Unsuccessful]:[%02d/%02d/%04d %02d:%02d:%02d]\n", local->tm_mday, local->tm_mon+1,local->tm_year+1900, local->tm_hour, local->tm_min, local->tm_sec);
          }
          else
              fprintf(rp, "[Add to list from file]:[Successful]:[%02d/%02d/%04d %02d:%02d:%02d]\n", local->tm_mday, local->tm_mon+1,local->tm_year+1900, local->tm_hour, local->tm_min, local->tm_sec);

      } else {
       printf("Failed to open file\n");
          time_t now;
          time(&now);
          struct tm *local = localtime(&now);
          fprintf(rp, "[Add to list from file]:[Unsuccessful]:[%02d/%02d/%04d %02d:%02d:%02d]\n", local->tm_mday, local->tm_mon+1,local->tm_year+1900, local->tm_hour, local->tm_min, local->tm_sec);
      }


}
void index() {
    struct Node* current = head;
    int index=1;
    while (index < (globalId-1)||current!=NULL) {
        current->data.id=index;
        current = current->next;
        index++;
    }

}
struct Node* searchNode(int key) {
    struct Node* current = head;
    while (current != NULL && current->data.id != key) {
        current = current->next;
    }

    return current;
}
void deleteNode(int num) {

    struct Node* del_node=searchNode(num);
    if (head == del_node) {
        head = del_node->next;
    }
    if (del_node->next != NULL) {
        del_node->next->prev = del_node->prev;
    }
    if (del_node->prev != NULL) {
        del_node->prev->next = del_node->next;
    }
    free(del_node);
}
void printList(FILE* fp) {
    printf("All your events:\n\n");
    printf("id  |    Name    |      Place      |    Members      |     Goals               |     Date                |\n..........................................................................................................\n");
    struct Node* current = head;
    if(current==NULL)
    {
        printf("There is not a note!\n");
        time_t now;
        time(&now);
        struct tm *local = localtime(&now);
        fprintf(fp, "[Show all notes]:[Unsuccessful]:[%02d/%02d/%04d %02d:%02d:%02d]\n", local->tm_mday, local->tm_mon+1,local->tm_year+1900, local->tm_hour, local->tm_min, local->tm_sec);
    }
    else {
        while (current != NULL) {
            printNote(current->data);
            printf("\n");
            current = current->next;
        }
        time_t now;
        time(&now);
        struct tm *local = localtime(&now);
        fprintf(fp, "[Show all notes]:[Successful]:[%02d/%02d/%04d %02d:%02d:%02d]\n", local->tm_mday, local->tm_mon+1,local->tm_year+1900, local->tm_hour, local->tm_min, local->tm_sec);
    }
}
void getLastNoteId()
{
    // struct Note note;
    //FILE *fp = fopen("notes.bin", "rb");

    //  if (fp != NULL) {
    //      while (fread(&note, sizeof(struct Note), 1, fp) == 1) {
    //         globalId = note.id;
    //     }
    //globalId++;
    //  if(note.id==0)globalId=1;
    //    fclose(fp);
    //  } else {
//        printf("Failed to open file\n");
    //}
    if (head!=NULL)
        globalId=(tail->data.id+1);
    else globalId=1;

}
void updateByID(int id) {
    struct Node *update_node = searchNode(id);
    printf("Select the parameter to measure the event:\n1)Name\n2)Place\n3)Members\n4)Goals\n5)Date\n");
    parameter num;
    scanf("%d", &num);

    switch (num) {
        case name: {
            printf("Enter structure name: ");
            fflush(stdin);
            scanf("%[^\n]", update_node->data.name);
            break;
        }
        case place:{
                    printf("Enter structure place: ");
                    fflush(stdin);
                    scanf("%[^\n]", update_node->data.description.place);
                    break;}
                case members:{
                    printf("Enter structure members: ");
                    fflush(stdin);
                    scanf("%[^\n]", update_node->data.description.members);
                    break;}
                case goals:{
                    printf("Enter structure goals: ");
                    fflush(stdin);
                    scanf("%[^\n]", update_node->data.description.goals);
                    break;}

        case date: {
            fflush(stdin);
            struct tm event_time;
            char input_str[100];
            int correct = 1;
            while (correct != 0) {
                printf("Enter the date and time of the event in the format dd.mm.yyyy hh:mm:ss: ");
                fgets(input_str, sizeof(input_str), stdin);
                sscanf(input_str, "%d.%d.%d %d:%d:%d\n", &event_time.tm_mday, &event_time.tm_mon,
                       &event_time.tm_year,
                       &event_time.tm_hour, &event_time.tm_min, &event_time.tm_sec);
                event_time.tm_mon -= 1;
                event_time.tm_year -= 1900;
                update_node->data.date = mktime(&event_time);
                time_t now;
                time(&now);
                struct tm *local = localtime(&now);
                int nowDate = mktime(local);
                if ((mktime(&event_time) == -1) || (nowDate > update_node->data.date)) {
                    printf("Error! Enter correct date: ");
                } else correct = 0;
                break;
            }
            break;
            }
            default: {
                printf("Error: you enter incorrect data!");
                system("pause");
                break;
            }
        }
    }
void writeToFile(struct Note note,FILE* fr)
{
    FILE *fp = fopen("notes.bin", "a+b");
    if (fp != NULL) {
       fwrite(&note, sizeof(struct Note), 1, fp);
       fclose(fp);
        time_t now;
        time(&now);
        struct tm *local = localtime(&now);
        fprintf(fr, "[Add to file from list]:[Successful]:[%02d/%02d/%04d %02d:%02d:%02d]\n", local->tm_mday, local->tm_mon+1,local->tm_year+1900, local->tm_hour, local->tm_min, local->tm_sec);

    } else {
        printf("Failed to open file\n");
        time_t now;
        time(&now);
        struct tm *local = localtime(&now);
        fprintf(fr, "[Add to file from list]:[Unsuccessful]:[%02d/%02d/%04d %02d:%02d:%02d]\n", local->tm_mday, local->tm_mon+1,local->tm_year+1900, local->tm_hour, local->tm_min, local->tm_sec);
    }
}
void dayOfWeek(FILE * fp){
    int day,temp=0;
    printf("Select day of week:\n1)MONDAY\n2)TUESDAY\n3)WEDNESDAY\n4)THURSDAY\n5)FRIDAY\n6)SATURDAY\n7)SUNDAY\n");
    struct Note note;
    scanf("%d",&day);
    while(getchar()!='\n'||day<1||day>7)
    { rewind(stdin);
        printf("error: enter correct date!\n");
        scanf("%d",&day);}
    if(day==7)day=0;
    struct Node* current = head;
    int index=1;
    while (index < globalId) {

        tm* aTm = localtime(&current->data.date);
        if(aTm->tm_wday==day){
            printNote(current->data);
            temp++;}

        current = current->next;
        index++;
    }
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    if(temp==0){printf("There are not any events in this day!\n");

        fprintf(fp, "[Finding event on certain day of the week]:[Unsuccessful]:[%02d/%02d/%04d %02d:%02d:%02d]\n", local->tm_mday, local->tm_mon+1,local->tm_year+1900, local->tm_hour, local->tm_min, local->tm_sec);
    }
    else    fprintf(fp, "[Finding event on certain day of the week]:[Successful]:[%02d/%02d/%04d %02d:%02d:%02d]\n", local->tm_mday, local->tm_mon+1,local->tm_year+1900, local->tm_hour, local->tm_min, local->tm_sec);
}
void listOfDay(FILE* fp){
    int temp=0,correct=1;

    fflush(stdin);
    struct tm event_time;
    char input_str[100];
    while(correct!=0){
        printf("Enter the date of the event in the format dd.mm.yyyy:   ");
        fgets(input_str, sizeof(input_str), stdin);
        sscanf(input_str, "%d.%d.%d\n", &event_time.tm_mday, &event_time.tm_mon, &event_time.tm_year);
        event_time.tm_mon -= 1; event_time.tm_year -= 1900;
        if((mktime(&event_time)==-1)) {
            printf("Error! Enter correct date: ");}
        else correct=0;
    }

    struct Node* current = head;
    int index=1;
    while (index < globalId) {

        tm* aTm = localtime(&current->data.date);
        if(aTm->tm_mday==event_time.tm_mday&&aTm->tm_mon==event_time.tm_mon&&aTm->tm_year==event_time.tm_year){
            printNote(current->data);
            temp++;}

        current = current->next;
        index++;
    }
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    if(temp==0) {
        printf("There are not any events in this day!\n");
        fprintf(fp, "[Finding event on certain day]:[Unsuccessful]:[%02d/%02d/%04d %02d:%02d:%02d]\n",
                local->tm_mday, local->tm_mon + 1, local->tm_year + 1900, local->tm_hour, local->tm_min, local->tm_sec);
    }
    else    fprintf(fp, "[Finding event on certain day]:[Successful]:[%02d/%02d/%04d %02d:%02d:%02d]\n", local->tm_mday, local->tm_mon+1,local->tm_year+1900, local->tm_hour, local->tm_min, local->tm_sec);

}
void addToFile(FILE* fp)
{
    struct Node* current = head;
    if(current==NULL)
    {
        time_t now;
        time(&now);
        struct tm *local = localtime(&now);
        fprintf(fp, "[Add to file from list]:[Unsuccessful]:[%02d/%02d/%04d %02d:%02d:%02d]\n", local->tm_mday, local->tm_mon+1,local->tm_year+1900, local->tm_hour, local->tm_min, local->tm_sec);
    }
    while (current!=NULL) {
        writeToFile(current->data,fp);
        current = current->next;
    }

}
long long scid()
{
    long long n, K;
    char a;
    while (true) {
        n = scanf("%lld", &K);
        scanf("%c", &a);
        if (n != 0 && a == '\n' && K > 0) {
            return K;
        }
        else {
            printf("Wrong data \n");
            rewind(stdin);
        }
    }
}
void SortData(int num, struct Node *&head, int parameter)
{
    struct Node* ptr1 = head;
    struct Node* ptr2 = head;
    switch (parameter)
    {
        case name:
            while (ptr1 != NULL)
            {
                while (ptr2 != NULL)
                {
                    if (strcmp(ptr2->data.name,ptr1->data.name)<0)
                    {
                        struct Note temp = ptr2->data;
                        ptr2->data = ptr1->data;
                        ptr1->data = temp;
                    }

                    ptr2 = ptr2->next;
                }
                ptr1 = ptr1->next;
                ptr2 = ptr1;
            }
            break;
        case place:

            while (ptr1 != NULL)
            {
                while (ptr2 != NULL)
                {
                    if (strcmp(ptr2->data.description.place,ptr1->data.description.place)<0)
                    {
                        struct Note temp = ptr2->data;
                        ptr2->data = ptr1->data;
                        ptr1->data = temp;
                    }

                    ptr2 = ptr2->next;
                }
                ptr1 = ptr1->next;
                ptr2 = ptr1;
            }
            break;
        case members:
            while (ptr1 != NULL)
            {
                while (ptr2 != NULL)
                {
                    if (strcmp(ptr2->data.description.members,ptr1->data.description.members)<0)
                    {
                        struct Note temp = ptr2->data;
                        ptr2->data = ptr1->data;
                        ptr1->data = temp;
                    }

                    ptr2 = ptr2->next;
                }
                ptr1 = ptr1->next;
                ptr2 = ptr1;
            }
            break;
        case goals:
            while (ptr1 != NULL)
            {
                while (ptr2 != NULL)
                {
                    if (strcmp(ptr1->data.description.goals, ptr2->data.description.goals) > 0)
                    {
                        struct Note temp = ptr2->data;
                        ptr2->data = ptr1->data;
                        ptr1->data = temp;
                    }

                    ptr2 = ptr2->next;
                }
                ptr1 = ptr1->next;
                ptr2 = ptr1;
            }
            break;
        case date:
            while (ptr1 != NULL)
            {
                while (ptr2 != NULL)
                {
                    if (ptr1->data.date > ptr2->data.date)
                    {
                        struct Note temp = ptr2->data;
                        ptr2->data = ptr1->data;
                        ptr1->data = temp;
                    }

                    ptr2 = ptr2->next;
                }
                ptr1 = ptr1->next;
                ptr2 = ptr1;
            }
            break;

    }
}
void MultiSortData(FILE* fp)
{
    printf("Enter amount of parameters:\n");
    int count;
    while (true)
    {
        count = scid();
        if (count > 5 || count < 1)
            printf("error:enter correct data:\n");
        else break;
    }
    int* choose = (int*)malloc(count * sizeof(int));
    printf("Select the parameter:\n1)Name\n2)Place\n3)Members\n4)Goals\n5)Data\n");
    for (int i = 0; i < count; i++)
    {
        while (true)
        {
            choose[i] = scid();
            if (choose[i] > 5 || choose[i] < 1)
                printf("error:enter correct data:\n");
            else break;
        }
    }
    for (int i = count-1; i >=0; i--) {
        switch (choose[i]) {
            case 1:
                SortData(count, head, name);
                break;
            case 2:
                SortData(count, head, place);
                break;
            case 3:
                SortData(count, head, members);
                break;
            case 4:
                SortData(count, head, goals);
                break;
            case 5:
                SortData(count, head, date);
                break;
        }
    }
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    fprintf(fp, "[Sort by %d parameter(-s)]:[Successful]:[%02d/%02d/%04d %02d:%02d:%02d]\n",count, local->tm_mday, local->tm_mon+1,local->tm_year+1900, local->tm_hour, local->tm_min, local->tm_sec);
}
typedef union {
    unsigned int num;
    struct {
        unsigned int bit0 : 1;
        unsigned int bit1 : 1;
        unsigned int bit2 : 1;
        unsigned int bit3 : 1;
        unsigned int bit4 : 1;
        unsigned int bit5 : 1;
        unsigned int bit6 : 1;
        unsigned int bit7 : 1;
    } bits;
} BinaryNum;
void divide(BinaryNum dividend, BinaryNum divisor, BinaryNum *remainder) {
    BinaryNum temp = {0};
    int num_bits =  8; // Количество бит в числе

    // Проверка на случай деления на ноль
    if (divisor.num == 0) {
        printf("Error: Division by zero");
        exit(1);
    }

    // Выполнение целочисленного деления столбиком
    for (int i = num_bits - 1; i >= 0; i--) {
        temp.num <<= 1;
        temp.bits.bit0 = dividend.bits.bit7;

        dividend.num <<= 1;
        if (temp.num >= divisor.num) {
            temp.num -= divisor.num;
        }
    }

    *remainder = temp;
}
void print_binary(BinaryNum num) {
    printf("%d%d%d%d%d%d%d%d", num.bits.bit7, num.bits.bit6, num.bits.bit5, num.bits.bit4,
           num.bits.bit3, num.bits.bit2, num.bits.bit1, num.bits.bit0);
}

typedef union {
    unsigned int num;
    unsigned int bit2 : 1;
    unsigned int bit4 : 2;
    unsigned int bit8 : 3;
    unsigned int bit16 : 4;
    unsigned int bit32 : 5;
    unsigned int bit64 : 6;
    unsigned int bit128 : 7;
}bit ;


void del(unsigned a,unsigned b) {
    bit value1,value2;
    value1.num=a;
    value2.num=b;
    int temp;
    switch (b) {
        case 2:
            temp=value1.bit2;
            break;
        case 4:
            temp=value1.bit4;
            break;
        case 8:
            temp=value1.bit8;
            break;
        case 16:
            temp=value1.bit16;
            break;
        case 32:
            temp=value1.bit32;
            break;
        case 64:
            temp=value1.bit64;
            break;
        case 128:
            temp=value1.bit128;
            break;
        default:
        { printf("With bit fields: incorrect data!");
            return;}
    }
    printf("With bit fields:\n%d mod %d = %d\n", value1.num,value2.num, temp);
}

int main(int argc,char** argv)
{
    FILE *fp = fopen("console.log", "a+");
    if (fp != NULL) {
    if (argc != 3) {
        printf("Usage: %s <dividend> <divisor>\n", argv[0]);
    }
    del(unsigned (atoi(argv[1])),unsigned (atoi(argv[2])));
    BinaryNum dividend = {unsigned (atoi(argv[1]))};
    BinaryNum divisor = {unsigned (atoi(argv[2]))};
    if(dividend.num>divisor.num) {
        printf("With bit operations:\n");
        BinaryNum remainder = {0};
        printf("Dividend: ");
        print_binary(dividend);
        printf("\nDivisor: ");
        print_binary(divisor);
        divide(dividend, divisor, &remainder);
        printf("\nRemainder (%d): ", remainder.num);
        print_binary(remainder);
        printf("\n\n\n");
    }
        time_t now;
        time(&now);
        struct tm *local = localtime(&now);
        fprintf(fp, "[Start the program]:[Successful]:[%02d/%02d/%04d %02d:%02d:%02d]\n", local->tm_mday, local->tm_mon+1,local->tm_year+1900, local->tm_hour, local->tm_min, local->tm_sec);
        fprintf(fp, "[Remainder is calculated]:[Successful]:[%02d/%02d/%04d %02d:%02d:%02d]\n", local->tm_mday, local->tm_mon+1,local->tm_year+1900, local->tm_hour, local->tm_min, local->tm_sec);
    int stop=0;
    while(stop!=10){
        printf("Choose command:\n1)show all notes\n2)add new note\n3)delete note\n4)update a note\n5)sort notes\n6)open events in certain day of week\n7)open events in certain day\n8)save notes in file\n9)write notes from file\nIf you want to complete the program, enter any other value!\n");
        commands command;
        char a;
        while (true) {
            scanf("%d", &command);
            scanf("%c", &a);
            if (command!= 0 && a == '\n' && command > 0) {
                break;
            }
            else {
                printf("Wrong data \n");
                rewind(stdin);
            }
        }
    getLastNoteId();

    switch(command){
        case show:    { system("cls");
            printList(fp);
            break;}
        case add:   { system("cls");
            printf("Enter new event:\n");
            insertAtEnd();
            time(&now);
            local = localtime(&now);
            fprintf(fp, "[Add new note]:[Successful]:[%02d/%02d/%04d %02d:%02d:%02d]\n", local->tm_mday, local->tm_mon+1,local->tm_year+1900, local->tm_hour, local->tm_min, local->tm_sec);
             break;}
        case dremove: {
            printf("Enter id      \n");
            int num;
             scanf("%d",&num);
             if(num>=globalId||num!=(int)num)
             { printf("Enter correct data!  ");
                 time(&now);
                 local = localtime(&now);
                 fprintf(fp, "[Delete the note]:[Unsuccessful]:[%02d/%02d/%04d %02d:%02d:%02d]\n", local->tm_mday, local->tm_mon+1,local->tm_year+1900, local->tm_hour, local->tm_min, local->tm_sec);}
          else {deleteNode(num);
          index();
                 time(&now);
                 local = localtime(&now);
                 fprintf(fp, "[Delete %d-th note]:[Successful]:[%02d/%02d/%04d %02d:%02d:%02d]\n", num,local->tm_mday, local->tm_mon+1,local->tm_year+1900, local->tm_hour, local->tm_min, local->tm_sec);}
           break;}
           case update: {
               system("cls");
               int num;
               printf("Enter id of note that you want to update:   \n");
               scanf("%d",&num);
               if(num>=globalId||num!=(int)num)
               { printf("Enter correct data!  ");
                   time(&now);
                   struct tm *local = localtime(&now);
                   fprintf(fp, "[Update the note]:[Unsuccessful]:[%02d/%02d/%04d %02d:%02d:%02d]\n", local->tm_mday, local->tm_mon+1,local->tm_year+1900, local->tm_hour, local->tm_min, local->tm_sec);}
               else {
                   updateByID(num);
                   time(&now);
                   struct tm *local = localtime(&now);
                   fprintf(fp, "[Update the %d-th note]:[Successful]:[%02d/%02d/%04d %02d:%02d:%02d]\n",num, local->tm_mday, local->tm_mon+1,local->tm_year+1900, local->tm_hour, local->tm_min, local->tm_sec);
               }
               break;
           }
           case sort:{
               system("cls");
               MultiSortData(fp);
               index();
               break;
           }
           case dweek : {system("cls");dayOfWeek(fp);
           break;}
           case day:{system("cls");
               listOfDay(fp);break;}
           case file:{addToFile(fp);break;}
               case list: {
                   addToList(fp);
                   break;}
        default:  {stop=10; break;}}
        printf("\n");
        system("pause");
            system("cls");
    }
    time(&now);
        local = localtime(&now);
        fprintf(fp, "[Exit from the program]:[Successful]:[%02d/%02d/%04d %02d:%02d:%02d]\n", local->tm_mday, local->tm_mon+1,local->tm_year+1900, local->tm_hour, local->tm_min, local->tm_sec);
        fclose(fp);
    } else {
        printf("Failed to open file\n");
    }
    fflush(stdin);
    getchar();
    return 0;
}

