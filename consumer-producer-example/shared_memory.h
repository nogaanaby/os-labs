//
//created by noga on 6/14/2022.
//

#ifndef CONSUMER_PRODUCER_EXAMPLE_SHARED_MEMORY_H
#include <stdbool.h>

//attach memory block
//associated with filename
//create it if it doesnt exist
char* attach_memory_block(char* filename, int size);
bool detach_memory_block(char *block);
bool destroy_memory_block(char *filename);

//all of the progrms will share these values
#define BLOCK_SIZE 4096
#define FILENAME "writeshmem.c"

#define CONSUMER_PRODUCER_EXAMPLE_SHARED_MEMORY_H

#endif //CONSUMER_PRODUCER_EXAMPLE_SHARED_MEMORY_H
