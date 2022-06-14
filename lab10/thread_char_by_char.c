#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define NUM_THREADS  	    5
#define NUM_LINES_PRINT     3

pthread_t tid[NUM_THREADS];

void* trythis(void* arg)
{
	int i, j;
        char tmp_str[50];
 

	for( i=0; i<NUM_LINES_PRINT; i++)
	{
		sprintf(tmp_str, "Hello world from child \n");
		for(j=0; j<strlen(tmp_str); j++)
		{
			printf("%c", tmp_str[j] );
			fflush(stdout);
		}
		printf("\n");
	}

    return NULL;
}


int main(void)
{
    int i;
    int error;
    
	for ( i = 0; i < NUM_THREADS; i++ ) 
	{
        // error = pthread_create(&(tid[i]), NULL, &trythis, thr_names[i]);
		error = pthread_create(&(tid[i]), NULL, &trythis, NULL);

        if (error != 0)
            printf("\nThread can't be created : [%s]", strerror(error));
     
    }

	for ( i = 0; i < NUM_THREADS; i++ ) 
		pthread_join(tid[i], NULL);        // wait for completion of the thread i
  
	return(0);
}

