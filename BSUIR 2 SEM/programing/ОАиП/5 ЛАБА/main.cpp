#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct FILO {
    float data;
    struct FILO *next;
};
struct Operator{
    int data;
    struct Operator *next;
};
struct Stack {
    char data;
    struct Stack* next;
};

struct FILO *push(struct FILO *head, float x) {
    struct FILO *n;
    n = (struct FILO*)malloc(sizeof(struct FILO));
    n->next = NULL;
    n->data = x;
    if (head == NULL)
        head = n;
    else
    {
        n->next = head;
        head = n;
    }
    return head;

}
struct Operator *push_operator(struct Operator *head, int x) {
    struct Operator *n;
    n = (struct Operator*)malloc(sizeof(struct Operator));
    n->next = NULL;
    n->data = x;
    if (head == NULL)
        head = n;
    else
    {
        n->next = head;
        head = n;
    }
    return head;

}
void push(struct Stack *s, char c) {
    struct Stack *n;
    n = (struct Stack*)malloc(sizeof(struct Stack));
    n->next = NULL;
    n->data = c;
    if (s == NULL)
        s = n;
    else
    {
        n->next = s;
        s = n;
    }
}

float pop(struct FILO ** head)
{
    if ((*head) == NULL) {
        printf("Error: Stack underflow\n");
        system("pause");
        exit(1);
    }
    if ((*head) != NULL)
    {   float num=(*head)->data;
        (*head) = (*head)->next;
        return num;
    }

}
int pop_operator(struct Operator ** head)
{
    if ((*head) == NULL) {
        printf("Error: Stack underflow\n");
        system("pause");
         exit(1);
    }
    if ((*head) != NULL)
    {   int num=(*head)->data;
        (*head) = (*head)->next;
        return num;
    }

}
char pop(struct Stack **s) {
    if ((*s)==NULL) {
        printf("Empty!\n");
        return NULL;
    }

    if ((*s) != NULL)
    {
        (*s) = (*s)->next;
        return (*s)->data;
    }
}

float evaluate_expression(char *expression) {
    struct FILO *head_operand=NULL;
    struct Operator *head_operator=NULL;


    int i;
    for (i = 0; expression[i] != '\0'; i++) {
        if (isdigit(expression[i])) {
            float operand = 0;
            while (isdigit(expression[i])) {
                operand = operand * 10 + (expression[i] - '0');
                i++;
            }
            i--;
         head_operand=push(head_operand, operand);
        } else if (expression[i] == '(') {
            head_operator=push_operator(head_operator, '(');
        } else if (expression[i] == ')') {
            while (head_operator->data != '(') {
                float operand2 = pop(&head_operand);
                float operand1 = pop(&head_operand);
                int operatore = pop_operator(&head_operator);
                float result;
                switch (operatore) {
                        case '+':
                        result = operand1 + operand2;
                        break;
                        case '-':
                        result = operand1 - operand2;
                        break;
                        case '*':
                        result = operand1 * operand2;
                        break;
                        case '/':
                        result = operand1 / operand2;
                        break;
                }
                    head_operand=push(head_operand, result);
            }
            pop_operator(&head_operator);
        } else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/' || expression[i] == '%') {
            while (head_operator != NULL && head_operator->data!='(' && ((expression[i] != '*' && expression[i] != '/') || (head_operator->data == '*' || head_operator->data == '/'))) {
                float operand2 = pop(&head_operand);
                float operand1 = pop(&head_operand);
                int operatore = pop_operator(&head_operator);
                float result;
                switch (operatore) {
                        case '+':
                        result = operand1 + operand2;
                        break;
                        case '-':
                        result = operand1 - operand2;
                        break;
                        case '*':
                        result = operand1 * operand2;
                        break;
                        case '/':
                        result = operand1 / operand2;
                        break;
                }
                   head_operand= push(head_operand, result);
            }
          head_operator= push_operator(head_operator, expression[i]);
        }
    }

    while (head_operator!= NULL) {
        float operand2 = pop(&head_operand);
        float operand1 = pop(&head_operand);
        int operatore = pop_operator(&head_operator);
        float result;
        switch (operatore) {
                case '+':
                result = operand1 + operand2;
                break;
                case '-':
                result = operand1 - operand2;
                break;
                case '*':
                result = operand1 * operand2;
                break;
                case '/':
                result = operand1 / operand2;
                break;
        }
           head_operand=push(head_operand, result);
    }

    return pop(&head_operand);

}
char* setString() {
    char* str=(char*)malloc(1);
    str[0]='\0';
    int i = 1, c;
    while ((c = getchar()) != EOF && c != '\n') {
        i++;
        str = (char*)realloc(str, i*sizeof(char));
        if (str != NULL) {
            str[i - 2] = c;
            str[i - 1] = '\0';
        }
    }
    return str;
}
int symbols(char c) {
    if((c>='0'&&c<='9')||c=='+'||c=='-'||c=='*'||c=='/'||c=='('||c==')'||c==' ')
        return 1;
    else
        return 0;
}

int isOpeningBracket(char c) {
    return c == '(';
}
int isClosingBracket(char c) {
    return c == ')';
}
int areMatchingBrackets(char opening, char closing) {
    return (opening == '(' && closing == ')') ;
}
int isBalanced(char *exp) {
    struct Stack *s=NULL;

    int i;
    for (i = 0; i < strlen(exp); i++) {
        if (isOpeningBracket(exp[i])) {
            push(s, exp[i]);
        } else if (isClosingBracket(exp[i])) {
            if (s==NULL || !areMatchingBrackets(pop(&s), exp[i])) {
                return 0;
            }
        }
    }
    return 1;
}

int main() {
    char* expression;
    printf("Enter an expression in infix notation: \n");
    expression=setString();
    int i=0,stop=1;
    while(expression[i]!='\0')
    {
        if(!(stop=symbols(expression[i]))) break;
        i++;
    }
    if(!(isBalanced(expression))&&stop==1){
    float temp=evaluate_expression(expression);
    printf("Result: %f\n",temp );}
    else printf("Error incorrect data!\n");
    system("pause");
    return 0;
}
