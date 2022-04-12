#include "stdio.h"
#include "stdlib.h"

//Function to calculate average time
void findavgTime(int *service_time, int num_of_processes, int quantum)
{
    int *waiting_time=malloc(num_of_processes*sizeof(int));

    printf("Processes   Burst time   Waiting time   Turn around time\n");

    int i=0,burst_time, total_turnaround_time=0, total_waiting_time=0, got_cpu_time=0;
    int num_of_waiting_processes=num_of_processes;
    while(num_of_waiting_processes>0){
      if(service_time[i]!=0)
      {
        got_cpu_time=(service_time[i]<=quantum) ? (service_time[i]) : (quantum);
        service_time[i]-=got_cpu_time;
        printf("   %d ", (i + 1));
        printf("           %d ", service_time[i]);

        if(service_time[i]==0){
          num_of_waiting_processes-=1;
        }
        
        waiting_time[i]+=burst_time;
       
        printf("             %d ",waiting_time);
        //printf("                %d\n",total_turnaround_time);

        total_turnaround_time+=burst_time+waiting_time;
        total_waiting_time+=waiting_time;
      }
      if(i<num_of_processes-1){
        i++;
      }else{
        i=0;
      }
    }


}


int main(){
  int num_of_processes;
  int quantum;

  printf("please type number of processes you wolud like to run: \n");
  scanf("%d",&num_of_processes);

  int *service_time=malloc(num_of_processes*sizeof(int));
  
  printf("please type the time quantum limit: \n");
  scanf("%d",&quantum);

  printf("please type each process service time: \n");

  for(int i=0;i<num_of_processes;i++){
    printf("%d: ", i+1);
    scanf("%d",service_time+i);
    printf("\n");
  }

  findavgTime(service_time, num_of_processes, quantum);
  free(service_time);
}


