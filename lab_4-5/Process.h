#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"

typedef struct Process{
    int pid;
    int burst_time;
    int arrival_time;
    int complition_time;
    int turn_around_time;
    int waiting_time;
    int got_cpu_time;
    bool hold;
    bool finish;
}Process;

void print_process(Process *p);