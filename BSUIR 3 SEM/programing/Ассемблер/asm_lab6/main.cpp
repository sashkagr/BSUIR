#include "stdio.h"
#include "windows.h"

#define SIZE 10
float array[SIZE];

void inputArray();
void outputArray();
void asmAlgorithm();

int main() {
	inputArray();
	printf("Input array: \n");
	outputArray();

	asmAlgorithm();

	printf("\nResult array: \n");
	outputArray();

	return 0;
}

void inputArray() {
	int res;
	printf("Input 10 elements: \n");

	for (int i = 0; i < SIZE; ++i) {
		do {
			printf("%d) ", i);
			res = scanf_s("%f", &array[i]);
			while (getchar() != '\n');
			if (res != 1 || array[i] == 0) printf("Invalid input\n");
		} while (res != 1 || array[i] == 0);
	}
}

void outputArray() {
	for (int i = 0; i < SIZE; ++i) {
		printf("%d) %.5f\n", i, array[i]);
	}
}

void asmAlgorithm() {
    __asm {
            finit           ; Инициализация сопроцессора FPU.

            xor ecx, ecx     ; Обнуление регистра ecx (используется для управления циклом).
            mov ecx, SIZE    ; Инициализация ecx размером массива.

            lea ebx, array   ; Загрузка адреса массива array в регистр ebx.

            calculate_loop:
            fld1             ; Загрузка значения 1.0 в стек FPU.
            fdiv [ebx]       ; Выполнение деления 1.0 на x (значение в массиве).
            fstp [ebx]       ; Сохранение результата обратно в массив.

            add ebx, 4       ; Сдвиг адреса на следующий элемент массива (размер элемента float).

            loop calculate_loop  ; Повторение цикла SIZE раз, уменьшая ecx на каждой итерации.

            fwait            ; Дожидаемся завершения всех операций FPU.
    }

}