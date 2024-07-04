#include <stdio.h>
#include <stdlib.h>


// Функция выполняет операцию * метода минимизации булевых функций Рота
char* multiplication(char* row1, char* row2, int num_vars)
{
    // Выделение памяти для полученной строки
    char* str = (char*)calloc(num_vars + 1, sizeof(char));

    // Выполнение умножения строк
    for (int i = 0; i < num_vars; i++)
    {
        if (row1[i] == row2[i])
            str[i] = row1[i];
        else {
            if (row1[i] == 'x' || row2[i] == 'x') {
                if (row1[i] == 'x')
                    str[i] = row2[i];
                else
                    str[i] = row1[i];
            }
            else
                str[i] = 'y';
        }
    }

    // Добавляем Null-terminate в конец строки
    str[num_vars] = '\0';

    return str;
}
// Функция считает количество 'у' в минтерме
int checkY(char* row, int num_vars)
{
    int count = 0;

    // Количество символов 'y' в строке
    for (int i = 0; i < num_vars; i++)
    {
        if (row[i] == 'y')
            count++;
    }

    // Если имеется более одного 'y', возвращается 0; в противном случае возвращается 1
    if (count > 1)
        return 0;

    return 1;
}
// Функция проверяет, покрывает ли(содержит) одна минтерма другую
int check(char* row1, char* row2, int num_vars)
{
    int count = 0;

    // Количество совпадающих символов в строках
    for (int i = 0; i < num_vars; i++)
    {
        if (row1[i] == row2[i])
            count++;
    }

    //Если все символы совпадают, возвращается 1
    if (count == num_vars)
        return 1;
    else {
        int amount = num_vars - count; // Количество несовпадающих символов
        int amountRow1 = 0;            // Количество 'х' в первой минтерме
        int amountRow2 = 0;            // Количество 'х' во второй минтерме

        // Сравнение количества 'х' в каждой минтерме с количеством несовпадающих переменных
        // Возвращаем 1, если первая минтерма покрывает вторую
        // Возвращаем 2, если вторая минтерма покрывает первую
        for (int i = 0; i < num_vars; i++)
        {
            if (row1[i] != row2[i]) {
                if (row1[i] == 'x')
                    amountRow1++;

                if (amountRow1 == amount)
                    return 1;

                if (row2[i] == 'x')
                    amountRow2++;

                if (amountRow2 == amount)
                    return 2;
            }
        }
    }

    return 0;
}
// Функция выполняет операцию *, выводит результат умножения, ищет новые кубы, выводит конечный результат
void multiplicationInRoth(char** minterms, int num_minterms, int num_vars)
{
    // Массив для хранения полученных минтерм
    char** minterms2 = (char**)calloc(1, sizeof(char*));
    int size = 0; // Размер массива

    // Выполнение операции умножения минтерм
    printf("Multiplication:\n");
    for (int i = 0; i < num_minterms; i++)
    {
        for (int j = i + 1; j < num_minterms; j++)
        {
            size++;
            minterms2 = (char**)realloc(minterms2, size * sizeof(char*));
            minterms2[size - 1] = multiplication(minterms[i], minterms[j], num_vars);
            printf("%s*%s=%s", minterms[i], minterms[j], minterms2[size - 1]);
            // Проверка на пустые множества
            if (checkY(minterms2[size - 1], num_vars) == 0)
            {
                size--;
                minterms2 = (char**)realloc(minterms2, size * sizeof(char*));
                printf("=empty set");
            }
            else {
                // Замена символа «y» на «x» в полученной минтерме
                for (int k = 0; k < num_vars; k++) {
                    if (minterms2[size - 1][k] == 'y') {
                        minterms2[size - 1][k] = 'x';
                    }
                }
                printf("=%s", minterms2[size-1]);
            }

            printf("\n");
        }
    }

    printf("\n");

    printf("Final minterms: ");

    // Добавляем все оставшиеся минтермы (в том числе те, которые не образовали новых кубов) в окончательные минтермы
    for (int i = 0; i < num_minterms; i++)
    {
        size++;
        minterms2 = (char**)realloc(minterms2, size * sizeof(char*));
        minterms2[size - 1] = minterms[i];
    }

    // Удаление лишних минтерм (повторяющихся и тех, которые образовали новые кубы)
    for (int i = 0; i < size; i++) {
        for (int k = i + 1; k < size; k++) {
            if (check(minterms2[i], minterms2[k], num_vars) == 1)
            {
                for (int j = k; j < size - 1; j++)
                    minterms2[j] = minterms2[j + 1];
                size--;
                minterms2 = (char**)realloc(minterms2, size * sizeof(char*));
                k--;
            }
            else if (check(minterms2[i], minterms2[k], num_vars) == 2)
            {
                for (int j = i; j < size - 1; j++)
                    minterms2[j] = minterms2[j + 1];
                size--;
                minterms2 = (char**)realloc(minterms2, size * sizeof(char*));
                i--;
            }
        }
    }

    // Выводим в консоль конечный результат
    printf("\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < num_vars; j++) {
            printf("%c", minterms2[i][j]);
        }
        if (i + 1 != size)
            printf(",");
    }
    printf("\n");
}
// Функция ввода строки с консоли
char* setString() {
    char* str = (char*)malloc(1);
    str[0] = '\0';
    int i = 1, c;

    // Считывание символов из ввода до тех пор, пока не встретится новая строка или конец файла
    while ((c = getchar()) != EOF && c != '\n') {
        i++;
        str = (char*)realloc(str, i * sizeof(char));
        if (str != NULL) {
            str[i - 2] = c;
            str[i - 1] = '\0';
        }
    }

    return str;
}
//Функция перевода цифровой последовательности типа string в число типа int
int aToI(char* str) {
    int result = 0;
    int i = 0;

    // Преобразование строки в целое число
    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    return result;
}
//Функция ввода строки и проверки на наличия только цифр
char* enterAndCheckNumberString(char* str)
{
    int flag = 0;

    while (!flag)
    {
        flag = 1;
        str = setString();
        int j = 0;

        // Проверка, содержит ли строка только цифры и имеет ли допустимую длину
        while (str[j] != '\0') {
            if (str[j] < '0' || str[j] > '9' || j > 8)
                flag = 0;
            j++;
        }
       //Вывод сообщения о вводе некорректной информации
        if (flag == 0||atoi(str)<2) {
            printf("Error: incorrect data!\nEnter new data\n");
            flag=0;
        }
    }

    return str;
}
//Функция ввода минтерм и проверки на наличия только бинарных цифр и символа 'х'
char** enterAndCheckMinterms(int num_minterms, int num_vars)
{
    char** minterms = (char**)calloc(num_minterms, sizeof(char*));

    for (int i = 0; i < num_minterms; i++)
    {
        printf("Enter %d-th minterm: \n", i + 1);
        int flag = 0;

        while (!flag) {
            flag = 1;
            minterms[i] = setString();
            int j = 0, count = 0;

            // Проверка, содержит ли минтерма только символы «0», «1» или «x» и имеет ли допустимую длину.
            while (minterms[i][j] != '\0') {
                count++;
                if ((minterms[i][j] < '0' || minterms[i][j] > '1') && (minterms[i][j] != 'x'))
                    flag = 0;
                j++;
            }

            if (count != num_vars)
                flag = 0;
            //Вывод сообщения о вводе некорректной информации
            if (flag == 0)
                printf("Error: incorrect data!\nEnter new data\n");
        }
    }

    return minterms;
}
int main() {
    char* num_vars;         // количество переменных в минтерме
    char* num_minterms;     // количество минтерм

    //Получение количество переменных с консоли
    printf("Enter number of variables: ");
    num_vars = enterAndCheckNumberString(num_vars);
    //Получение количество минтерм с консоли
    printf("Enter number of minterms: ");
    num_minterms = enterAndCheckNumberString(num_minterms);

    // Преобразование входных строк в целые числа
    int num_vars_int = aToI(num_vars);
    int num_minterms_int = aToI(num_minterms);

    // Получение минтерм с консоли
    printf("Enter minterms:\n");
    char** minterms = enterAndCheckMinterms(num_minterms_int, num_vars_int);

    // Выполнение умножения в алгоритме Рота
    multiplicationInRoth(minterms, num_minterms_int, num_vars_int);

    system("pause");
    return 0;
}
