#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include <stdbool.h>


int keyboardInput(Process processes[]) {
    int numProc = 0;
    printf("Введите количество процессов: ");
    scanf("%d", &numProc);

    for (int i = 0; i < numProc; i++) {
        processes[i].id = i + 1;

        printf("Введите время исполнения процесса %d: ", i + 1);
        scanf("%d", &processes[i].burstTime);

        printf("Введите количество предпроцессов: ");
        scanf("%d", &processes[i].numDepend);

        if (processes[i].numDepend) {
            processes[i].dependencies = (int*)malloc(processes[i].numDepend * sizeof(int));
            printf("Введите ID предпроцессов процесса %d: ", i + 1);
            for (int j = 0; j < processes[i].numDepend; j++)
                scanf("%d", &processes[i].dependencies[j]);
        }
    }

    return numProc;
}

int fileInput(Process processes[]) {
    FILE* fp = fopen("data.txt", "r");
    if (fp == NULL) {
        printf("Не удалось открыть файл");
        return 0;
    }

    int numProc;
    fscanf(fp, "%d", &numProc);

    for (int i = 0; i < numProc; i++) {
        processes[i].id = i + 1;

        fscanf(fp, "%d", &processes[i].burstTime);

        fscanf(fp, "%d", &processes[i].numDepend);
        processes[i].dependencies =(int*)malloc(processes[i].numDepend * sizeof(int));
        for (int j = 0; j < processes[i].numDepend; j++)
            fscanf(fp, "%d", &processes[i].dependencies[j]);
    }
    return numProc;
}




int calcProcTime(Process processes[], int numProc) {
    int* earlyStart = (int*)calloc(numProc, sizeof(int));
    int* lateStart = (int*)calloc(numProc, sizeof(int));
    int* earlyFinish = (int*)calloc(numProc, sizeof(int));
    int* lateFinish = (int*)calloc(numProc, sizeof(int));
    bool* isVisited = (bool*)calloc(numProc, sizeof(bool));

    for (int i = 0; i < numProc; i++) {
        if (processes[i].numDepend == 0) {
            earlyStart[i] = 0;
            earlyFinish[i] = processes[i].burstTime;
        }
        else {
            int maxPrevFinish = 0;
            for (int j = 0; j < processes[i].numDepend; j++) {
                int dependencyID = processes[i].dependencies[j];
                if (earlyFinish[dependencyID - 1] > maxPrevFinish) {
                    maxPrevFinish = earlyFinish[dependencyID - 1];
                }
            }
            earlyStart[i] = maxPrevFinish;
            earlyFinish[i] = maxPrevFinish + processes[i].burstTime;
        }
    }

    lateFinish[numProc - 1] = earlyFinish[numProc - 1];
    lateStart[numProc - 1] = earlyStart[numProc - 1];
    isVisited[numProc - 1] = true;

    for (int i = numProc - 2; i >= 0; i--) {
        int minNextFinish = INT_MAX;
        for (int j = 0; j < processes[i].numDepend; j++) {
            int dependencyID = processes[i].dependencies[j];
            if (lateStart[dependencyID - 1] < minNextFinish) {
                minNextFinish = lateStart[dependencyID - 1];
            }
        }

        lateFinish[i] = minNextFinish;
        lateStart[i] = lateFinish[i] - processes[i].burstTime;
        isVisited[i] = true;
    }

    int lenCritPath = 0;
    for (int i = 0; i < numProc; i++) {
        if (isVisited[i] && lateFinish[i] - earlyFinish[i] == 0) {
            if (earlyFinish[i] > lenCritPath) {
                lenCritPath = earlyFinish[i];
            }
        }
    }

    printf("ID     t_рн     t_рк\n");

    for (int i = 0; i < numProc; i++) {
        printf("%d\t %d\t %d\t ", i + 1, earlyStart[i], earlyFinish[i]);
        printf("\n");
    }



    free(earlyStart);
    free(lateStart);
    free(earlyFinish);
    free(lateFinish);
    free(isVisited);

    return lenCritPath;
}

