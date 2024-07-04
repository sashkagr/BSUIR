#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <windows.h>
void textcolor (int color)
{
    static int __BACKGROUND;

    HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;


    GetConsoleScreenBufferInfo(h, &csbiInfo);

    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE),
                             color + (__BACKGROUND << 4));
}


void printMatrix(int** matrix,int M, int N) {
    printf("Matrix:\n");
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {

            int temp=matrix[i][j];
            if(temp>10)
              while(temp>10)
                    temp-=10;

           switch(temp){
               case 0:
                   textcolor(3);
                  break;
                case 1:
                   textcolor(4);
                   break;
                case 2:
                    textcolor(14);
                    break;
                case 3:
                    textcolor(5);
                    break;
                case 4:
                    textcolor(6);
                    break;
                case 5:
                    textcolor(13);
                    break;
                case 6:
                    textcolor(8);
                    break;
                case 7:
                    textcolor(7);
                    break;
                case 8:
                    textcolor(10);
                    break;
                case 9:
                    textcolor(15);
                    break;
                case 10:
                    textcolor(12);
                    break;
          }
            printf("%02d ", matrix[i][j]);
        }
        printf("\n");
    }
    textcolor(15);
}

void floodFill(int*** matrix,int M, int N, int i, int j, int color) {
    if (i < 0 || i >= M || j < 0 || j >= N)
        return;
    if ((*matrix)[i][j] != 1)
        return;

    (*matrix)[i][j] = color;

    floodFill(matrix,M,N, i + 1, j, color);
    floodFill(matrix,M, N,i - 1, j, color);
    floodFill(matrix,M,N, i, j + 1, color);
    floodFill(matrix,M,N, i, j - 1, color);
}

void colorIslands(int*** matrix,int M, int N) {
    int color = 2;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if ((*matrix)[i][j] == 1) {
                floodFill(matrix,M,N, i, j, color);
                color++;
            }
        }
    }
}

int main() {
    int M, N;
    printf("Enter M: ");
    scanf("%d",&M);
    while (getchar() != '\n'  || M > 40||M<1)
    {
        rewind(stdin);
        printf("Error: please enter correct data:\n");
        scanf("%d", &M);
    }
    printf("Enter N: ");
    scanf_s("%d", &N);
    while (getchar() != '\n' || N > 40||N<1)
    {
        rewind(stdin);
        printf("Error: please enter correct data:\n");
        scanf_s("%d", &N);
    }


    srand(time(NULL));
    int** matrix=(int**)calloc(M,sizeof(int*));
    for(int i=0;i<M;i++)
        matrix[i]=(int*)calloc(N,sizeof(int));


    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 2;
        }
    }


    printMatrix(matrix,M,N);
    colorIslands(&matrix,M,N);
    printMatrix(matrix,M,N);

 system("pause");
    return 0;
}