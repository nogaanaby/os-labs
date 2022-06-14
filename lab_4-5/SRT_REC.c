#include "Process.h"

/*************************************BOTH***********************************/


void print_process(Process *p){
    printf("    %d              %d           %d              %d                %d                %d       \n", p->pid,p->arrival_time, p->burst_time, p->complition_time, p->waiting_time, p->turn_around_time);
}

void calc_WT_and_TAT(Process *p){
    p->turn_around_time=p->complition_time - p->arrival_time;
    p->waiting_time= p->turn_around_time - p->burst_time;

}

void print_avges(Process *processes,int num_of_processes){
    int total_wait_time=0, total_turn_around_time=0, throughtput=0;

    for(int i=0;i<num_of_processes; i++){
        total_wait_time+=(processes+i)->waiting_time;
        total_turn_around_time+=(processes+i)->turn_around_time;
    }

    printf(" avg wait time: %d \n",total_wait_time/num_of_processes);
    printf(" avg turn around time: %d \n",total_turn_around_time/num_of_processes);
    printf(" throught-put: %d \n",throughtput);
}
void print_processes(int num_of_processes, Process *processes){
    printf("Processes_id   Arrival_time   Burst_time   Complition_time   Waiting_time   Turn_around_time\n");
    for(int i=0;i<num_of_processes;i++){
        calc_WT_and_TAT(processes+i);
        print_process(processes+i);
    }

    print_avges(processes, num_of_processes);
}

void reset_processes(int num_of_processes,Process *processes){
    for(int i=0;i<num_of_processes;i++){
        (processes+i)->waiting_time=0;
        (processes+i)->turn_around_time=0;
        (processes+i)->complition_time=0;
        (processes+i)->got_cpu_time=0;
        (processes+i)->finish=false;
        (processes+i)->hold=false;
    }
}
/************************************************************************/
/***********************************SRT*************************************/
int choose_shortest_process(int current_time, Process *processes,int total_processes, int current_pid ){
    int min_burst_time=-1;
    int next_pid=current_pid;
    for(int i=0;i<total_processes;i++){
        if ((processes+i)->hold && (min_burst_time==-1 || (processes+i)->burst_time-(processes+i)->got_cpu_time < min_burst_time)) {
            min_burst_time=(processes+i)->burst_time;
            next_pid=(processes+i)->pid;
        };
    }
    return next_pid;
}
void update_hold_status(int time_counter, Process *processes, int total_processes){
    for(int i=0;i<total_processes;i++){
        if( !(processes+i)->finish && (processes+i)->arrival_time==time_counter ){
            (processes+i)->hold=true;
        }
    }
}

int SRT(int time_counter, Process *processes,int num_of_processes_left, int pid, int total_processes){
    if(num_of_processes_left!=0){
        update_hold_status(time_counter-1,processes, total_processes);
        int next_pid=choose_shortest_process(time_counter, processes,total_processes, pid);
        if( (processes+pid)->burst_time == (processes+pid)->got_cpu_time && !(processes+pid)->finish){
            printf("%d finished at %d \n", (processes+pid)->pid, time_counter);
            (processes+pid)->complition_time=time_counter;
            (processes+pid)->finish=true;
            (processes+pid)->hold=false;
            printf(" num_of_processes_left: %d \n",num_of_processes_left-1);
            SRT(time_counter, processes, num_of_processes_left-1, next_pid, total_processes);
        }else{
            (processes+next_pid)->got_cpu_time++;
            printf(" pid: %d, current time: %d, cpu time left: %d\n",(processes+next_pid)->pid,time_counter,(processes+next_pid)->burst_time-(processes+next_pid)->got_cpu_time);
            SRT(time_counter+1, processes, num_of_processes_left, next_pid, total_processes);
        }
    }
    return time_counter;

}

int SPN(int time_counter, Process *processes,int num_of_processes_left, int pid, int total_processes){
    if(num_of_processes_left!=0){
        update_hold_status(time_counter-1,processes, total_processes);
        if( (processes+pid)->burst_time == (processes+pid)->got_cpu_time && !(processes+pid)->finish){
            printf("%d finished at %d \n", (processes+pid)->pid, time_counter);
            (processes+pid)->complition_time=time_counter;
            (processes+pid)->finish=true;
            (processes+pid)->hold=false;
            printf(" num_of_processes_left: %d \n",num_of_processes_left-1);
            int next_pid=choose_shortest_process(time_counter, processes,total_processes, pid);
            SPN(time_counter, processes, num_of_processes_left-1, next_pid, total_processes);
        }else{
            (processes+pid)->got_cpu_time++;
            printf(" pid: %d, current time: %d, cpu time left: %d\n",(processes+pid)->pid,time_counter,(processes+pid)->burst_time-(processes+pid)->got_cpu_time);
            SPN(time_counter+1, processes, num_of_processes_left, pid, total_processes);
        }
    }
    return time_counter;

}


/************************************************************************/
/*********************************RR************************************/
// int RR( int total_processes,Process *processes,int quantum){
//     int num_of_processes_left=total_processes;
//     int i=0;
//     while(num_of_processes_left!=0){
//         if((processes+i)->burst_time<=quantum){
//             (processes+i)->got_cpu_time=(processes+i)->burst_time;
//         }else{
//             (processes+i)->got_cpu_time=quantum;
//         }
//     }
// }


/************************************************************************/


int main(){
    int num_of_processes, quantum;
    printf("please type number of processes you wolud like to run: \n");
    scanf("%d",&num_of_processes);

    //queue
    Process *processes=malloc(num_of_processes*sizeof(Process));

    printf("please type each process burst time: \n");

    for(int i=0;i<num_of_processes;i++){
        (processes+i)->pid=i;
        (processes+i)->got_cpu_time=0;
        (processes+i)->hold=false;
        (processes+i)->finish=false;
        printf("%d: ", i);
        scanf("%d",&(processes+i)->burst_time);

        printf("\n");
    }

    printf("please type each process arrival time: \n");

    for(int i=0;i<num_of_processes;i++){
        printf("%d: ", i);
        scanf("%d",&(processes+i)->arrival_time);
        printf("\n");
    }

    // printf("please type the time quantum limit for RR: \n");
    // scanf("%d",&quantum);


    // printf("***********************RR**************************\n");
    // RR();
    // print_processes(num_of_processes,processes);

    // reset_processes(num_of_processes,processes);

    printf("***********************SRT**************************\n");
    SPN(0,processes,num_of_processes,0,num_of_processes);
    print_processes(num_of_processes,processes);
}
