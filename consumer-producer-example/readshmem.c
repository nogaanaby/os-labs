#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shared_memory.h"

int main(int argc, char *argv[]){
    if(argc != 1){
        printf("usage - %s //no args", argv[0]);
        return -1;
    }

   char *block = attach_memory_block(FILENAME,BLOCK_SIZE);
    if(block == NULL){
        printf("EROOR - couldn`t get block");
        return -1;
    }
    
    printf("Reading \"%s\"\"n", block);
    detach_memory_block(block);
    
    return 0;
}