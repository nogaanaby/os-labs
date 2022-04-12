#include "stdio.h"
#include "stdlib.h"

int choose_shortest_process(int sooner_arrival_time,int num_of_waiting_processes, int *service_time, int *arrival_time, int *pids){
    int min_service_time=NULL;
    int next_pid=NULL;
    for(int i=0;i<num_of_waiting_processes;i++){
        if (arrival_time[i]==sooner_arrival_time && (min_service_time==NULL || service_time[i]<min_service_time)){
            min_service_time=service_time;
            next_pid=pids[i];
        };
    }
    return next_pid;
}



//Function to calculate average time
void findavgTime(int num_of_processes,int *pids, int *service_time, int *arrival_time)
{
    int waiting_time=0;    
    int burst_time, total_time=0;
    int num_of_waiting_processes=num_of_processes;
    int current_time=0;
    int sooner_arrival_time=0;

    printf("Processes PID   Arrival time   Burst time   Waiting time   Turn around time\n");


    while(num_of_waiting_processes>0){
        int next_pid=choose_shortest_process(sooner_arrival_time,num_of_waiting_processes,arrival_time,pids);
        if(next_pid==NULL){
            sooner_arrival_time++;
        }else{
            int process_index=find_process_index(next_pid);
            int running_time=calc_runnning_time(service_time[process_index]);
            print_process_details(, next_pid, );
            
            if(service_time[process_index]==running_time){
               num_of_waiting_processes= remove_process(process_index,pids,service_time,arrival_time);
            }else{
                service_time[process_index]-=running_time;
            }
    
        }
    }
        
   

}


int main(){
  int num_of_processes;

  printf("please type number of processes you wolud like to run: \n");
  scanf("%d",&num_of_processes);

  int *service_time=malloc(num_of_processes*sizeof(int));
  int *pids=malloc(num_of_processes*sizeof(int));


  printf("please type each process service time: \n");

  for(int i=0;i<num_of_processes;i++){
    printf("%d: ", i+1);
    scanf("%d",service_time+i);
    (pids+i)=i;
    printf("\n");
  }
  
  int *arrival_time=malloc(num_of_processes*sizeof(int));

  printf("please type each process arrival time: \n");

  for(int i=0;i<num_of_processes;i++){
    printf("%d: ", i+1);
    scanf("%d",arrival_time+i);
    printf("\n");
  }

  findavgTime(num_of_processes,pids,service_time,arrival_time);
  free(service_time);
  free(arrival_time);
  free(pids);
}


