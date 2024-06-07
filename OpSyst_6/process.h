struct Process {
    int id;
    int burstTime;
    int* dependencies;
    int numDepend;
};

int keyboardInput(Process processes[]);
int fileInput(Process processes[]);

int calcProcTime(Process processes[], int numProc);
