cmake_minimum_required(VERSION 3.21)
project(consumer_producer_example C)

set(CMAKE_C_STANDARD 99)

add_executable(consumer_producer_example shared_memory.h shared_memory.c readshmem.c writeshmem.c destroyshmem.c)

//gcc readshmem.c shared_memory.h shared_memory.c -o readshmem