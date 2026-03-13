#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct PCB {
    char* PID;
    int ARR_TIME;
    int BURST_TIME;
    int REM_TIME;
    int PRIORITY;
    char STATE[12];
    int CPU_START;
    int CPU_END;
} PCB;

PCB* create_process(char* PID, int ARR_TIME, int BURST_TIME, int PRIORITY) {
    PCB* new_process = (PCB*)malloc((sizeof(PCB)));
    new_process->PID = (char*)malloc(strlen(PID) + 1);
    strcpy(new_process->PID,PID);

    new_process->ARR_TIME = ARR_TIME;
    new_process->BURST_TIME = BURST_TIME;
    new_process->REM_TIME = BURST_TIME;
    new_process->PRIORITY = PRIORITY;

    strcpy(new_process->STATE, "NEW");
    return new_process;
}

void print_process(PCB* process) {
    printf("PID: %s, Arr: %d, Burst: %d, Rem %d, Prio %d, State: %s\n", process->PID, process->ARR_TIME, process->BURST_TIME, process->REM_TIME, process->PRIORITY, process->STATE);
}

int main() {
    PCB* processes[4];
    FILE* file = fopen("inputs.txt", "r");

    char PID[12];
    int ARR_TIME, BURST_TIME, PRIORITY;
    int index = 0;

    char line_buffer[1024];
    while (fgets(line_buffer, 1024, file) != NULL) {
       if (sscanf(line_buffer, "%s %d %d %d", PID, &ARR_TIME, &BURST_TIME, &PRIORITY) == 4) {
            processes[index++] = create_process(PID, ARR_TIME, BURST_TIME, PRIORITY);
       }
    }

    for (int i = 0; i < sizeof(processes) / sizeof(processes[0]); i++) {
        print_process(processes[i]);
    }
}