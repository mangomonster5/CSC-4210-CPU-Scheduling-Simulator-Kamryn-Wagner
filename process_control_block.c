#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct PCB {
    char* PID;
    int ARR_TIME;
    int BURST_TIME;
    int REM_TIME;
    int PRIORITY;
    char STATE[12]; //NEW, READY, RUNNING, or TERMINATED
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

void update_process(PCB* process, char* new_state) {
    strcpy(process->STATE, new_state);
    process->CPU_END++;
    print_process(process);
}

void increment_time(PCB* process) {
    if (strcmp(process->STATE, "RUNNING") != 0) return;

    process->REM_TIME--;

    if (process->REM_TIME == 0) {
        update_process(process, "TERMINATED");
    } 
}

void first_come_first_serve(PCB* processes[], int proc_count, int time) {
    int process_running = 1;
    for (int proc_index = 0; proc_index < proc_count; proc_index++) {
        if (strcmp(processes[proc_index]->STATE, "RUNNING") == 0) {
            process_running = 0;
            break;
        }
    }

    if (process_running) {
        for (int proc_index = 0; proc_index < proc_count; proc_index++) {
            PCB* current_proc = processes[proc_index];
            if (strcmp(current_proc->STATE, "READY") == 0) {
                update_process(current_proc, "RUNNING");
                break;
            }
        }
    }
}

void shortest_time_remaining(PCB* processes[], int proc_count, int time) {
    PCB* shortest = NULL;
    for (int proc_index = 0; proc_index < proc_count; proc_index++) {
        PCB* current_proc = processes[proc_index];
        if (strcmp(current_proc->STATE, "READY") == 0 || strcmp(current_proc->STATE, "RUNNING") == 0) {
            if (shortest == NULL || current_proc->REM_TIME < shortest->REM_TIME) {
                shortest = current_proc;
            }
        }
    }
    

    for (int proc_index = 0; proc_index < proc_count; proc_index++) {
        PCB* current_proc = processes[proc_index];
        if (strcmp(current_proc->STATE, "RUNNING") == 0 && current_proc != shortest) {
            update_process(current_proc, "READY");
        }
    }

    if (shortest != NULL && strcmp(shortest->STATE, "READY") == 0) {
        update_process(shortest, "RUNNING");
    }

}

void begin_clock(PCB* processes[], int proc_count, int total_time) {
    for (int time = 0; time < total_time; time++) {
        printf("\nTime: %d\n", time);
        for (int proc_index = 0; proc_index < proc_count; proc_index++) {
            PCB* current_proc = processes[proc_index]; 
            if (current_proc->ARR_TIME == time && strcmp(current_proc->STATE, "NEW") == 0) {
                update_process(current_proc, "READY");
            }
        }
        
        //first_come_first_serve(processes, proc_count, time);
        shortest_time_remaining(processes, proc_count, time);

        for (int proc_index = 0; proc_index < proc_count; proc_index++) {
            PCB* current_proc = processes[proc_index];
            if (strcmp(current_proc->STATE, "NEW") != 0 && strcmp(current_proc->STATE, "TERMINATED") != 0) {
                increment_time(current_proc);
            }
        }
    }
}

int main() {
    PCB* processes[4];
    FILE* file = fopen("inputs_folder/inputs.txt", "r");

    char PID[12];
    int ARR_TIME, BURST_TIME, PRIORITY;
    int index = 0;
    

    char line_buffer[1024];
    while (fgets(line_buffer, 1024, file) != NULL) {
       if (sscanf(line_buffer, "%s %d %d %d", PID, &ARR_TIME, &BURST_TIME, &PRIORITY) == 4) {
            processes[index++] = create_process(PID, ARR_TIME, BURST_TIME, PRIORITY);
       }
    }

    int proc_count = sizeof(processes) / sizeof(processes[0]);
    printf("-- INITIAL PROCESSES --\n");
    for (int i = 0; i < proc_count; i++) {
        print_process(processes[i]);
    }
    fclose(file);

    begin_clock(processes, proc_count, 12);
    return 0;
}