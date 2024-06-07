#define _CRT_SECURE_NO_WARNINGS
#define N 100

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "process.h"

int main() {
	setlocale(LC_ALL, "Rus");

	Process processes[N];
	int numProc = 0, choice = 0;

	printf("����:\n");
	printf("1. ���� � ����������\n");
	printf("2. ������ �� �����\n");
	printf("��� �����: ");
	scanf("%d", &choice);

	switch (choice) {
	case 1:
		numProc = keyboardInput(processes);
		break;
	case 2:
		numProc = fileInput(processes);
		break;
	default:
		printf("������ %d � ���� ���!", choice);
		return 0;
	}

	int lenCritPath = calcProcTime(processes, numProc);
	printf("\n����� ������������ ����: %d", lenCritPath);
}