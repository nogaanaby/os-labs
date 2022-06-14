#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>         // malloc & exit
#include <stdint.h>
#include <math.h>   

#define VIRTUAL_ADDRESS_BITS          16           //  64KB
#define PAGE_SIZE                     1024         // chosen to have small single level PT 
#define PHY_MEM_SIZE                 16*1024      // in B     

// computed 
int NumFrames = PHY_MEM_SIZE / PAGE_SIZE;
int NumPages = (1 << VIRTUAL_ADDRESS_BITS) / PAGE_SIZE;

struct PT_ENTRY {
    int frame_num;
    char not_allowed;                 // bool - not allowed by the process
    char in_ram;                      // bool - 1 in RAM 0 - not  RAM
    char changed;                    // bool - 1 was changed  0 - was not changed
};
typedef struct PT_ENTRY pt_entry;


struct FRAME_INFO {
    int busy;                        // holds a page
};
typedef struct FRAME_INFO frame_info;


// Lofical address structure
int first_level_bits, second_level_bits, offset_bits;
void test_single_write_read(int page, int offset, char value);
void write_byte(int log_addr, char value);
char read_byte(int log_addr);

int getLogicalAddr(int pt, int offst)
{
    return((pt << offset_bits) + offst);
}

int PT_num_entries;

pt_entry* PT_table;
frame_info* FrameInfo;

char* PhysMem;
char* VirMem;



//  read from the address + write to the address + read again
void test_single_write_read(int page, int offset, char value)
{
    printf("SINGLE WRITE_READ TEST ====> \n");
    int LogicalAddr = getLogicalAddr(page, offset);                // address to access
    char ch = read_byte(LogicalAddr);

    printf("\t\t Write value %d to logical addr %x (current value: %d) \n", value, LogicalAddr, ch);
    write_byte(LogicalAddr, value);

    ch = read_byte(LogicalAddr);
    if (ch == value)
        printf("\t\tRead value is the same as was written \n");
    else
        printf("\t\tRead value is NOT the same as was written !!!!! ERROR \n");
}


#define PT_VAL(addr)  (addr >> offset_bits)
#define OFFSET_VAL(addr)  (addr &  ((1 << offset_bits) -1 ) ) 
#define PHY_ADDR(frame_num, offset)  ( frame_num * PAGE_SIZE + offset )


/*
    Get the Physical address for the given Logical (virtual) address ( if needed bring the 
    page into the physical memory)
    Input:
        - addr      -  Logical address
        - w_operation  -   1 - operation is WRITE  - 0 - operation is READ
    Output:
        - the Physical address   (-1 - in case the page is not allowed)
*/
int logical2physical(int addr, char w_operation)
{
    printf("  logical2physical: 0x%x ( PT=%x offs=%x ) => \n", addr,
                        PT_VAL(addr), OFFSET_VAL(addr));

    //  HERE: put your code
        //read from page table PT_VAL(addr)
    return 1;

}

/*
    Write a BYTE to a Logical address.
*/
void write_byte(int log_addr, char value)
{
    int phy_addr = logical2physical(log_addr, 1);
    *(PhysMem + phy_addr) = value;
}

/*
    Read a BYTE from a Logical address.
    Return value:  the value in teh memory
*/
char read_byte(int log_addr)
{
    int phy_addr = logical2physical(log_addr, 0);
    return(*(PhysMem + phy_addr));
}


/*
    Get a free frame (for a Page to be loaded).
    If there is no free frame: make one of the frames to be free  - THIS IS CURRENLY NOT IMPLEMENTED.
    Current version:
        - if no free Frame, just EXIT.
*/
int getFrame()
{
    for (int i = 0; i < NumFrames; i++)
    {
        if (!FrameInfo[i].busy)
            return(i);
    }

    printf("No free Frames:  need to SWAP - CURREBTLY NOT SUPPORTED \n");
    exit(0);     // JUST EXIT: no need to check
}

// if 'only_in_RAM_flag'  - print ONLY those in RAM
int printPageTable(int only_in_RAM_flag)
{
    printf("==== printPageTable: printing ONLY pages that are allowed and are in MEMORY !!! \n");

    for (int i = 0; i < PT_num_entries; i++)
    {
        if (only_in_RAM_flag)
        {
            if (PT_table[i].in_ram)
            {
                printf(" entry %d is in RAM (frame num =%d)\n", i, PT_table[i].frame_num);
            }
        }
        else
        {
            if (!PT_table[i].not_allowed)
            {
                printf("PT entry %d is allowed -> ", i);
                if (PT_table[i].in_ram)
                    printf(" is in RAM (frame num =%d)\n", PT_table[i].frame_num);
                else
                    printf(" is not in RAM \n");
            }
        }
    }

    printf("====\n\n");
}

void main(void)
{
    int frame_size = PAGE_SIZE;    // constant  PhysMemorySize / FRAMES_NUMBER;
    offset_bits = (int)(ceil(log(frame_size) / log(2)));
    int virtual_mem_size = 1 << VIRTUAL_ADDRESS_BITS;

    printf("Configuration: \n");
    printf("    RAM size = %d Virtual mem size = %d Num frames = %d Num pages = %d \n",
        PHY_MEM_SIZE, virtual_mem_size, NumFrames, 1 << (VIRTUAL_ADDRESS_BITS - offset_bits));
    printf("    frame size = %d (bits for offset = %d) \n", frame_size, offset_bits);

    //  frame number bits (always 5 in our case) + 3 control bits (rounded to 8 bytes)
    int page_entry_size = (log(NumFrames) / log(2) + 3);
    page_entry_size = (page_entry_size % 8) ? (page_entry_size / 8 + 1) : page_entry_size / 8;
    printf("    size of Page Table entry (in bytes) = %d \n", page_entry_size);

    // single level Page Table size: PT entry size * number of Pages 
    int page_bits = VIRTUAL_ADDRESS_BITS - offset_bits;
    printf("    size of single level PT = %d Bytes \n", (1 << page_bits) * page_entry_size);

    PT_num_entries = frame_size / page_entry_size;
    if (PT_num_entries > NumPages)
        PT_num_entries = NumPages;

    // allocate tables
    PT_table = (pt_entry*)malloc(PT_num_entries * sizeof(pt_entry));     // PT table
    FrameInfo = (frame_info*)malloc(NumFrames * sizeof(frame_info));

    if (!PT_table || !FrameInfo)
    {
        printf("fail allocations !!! \n");
        return;
    }

    // allocat simulated Physical & Virtual memories
    PhysMem = (char*)calloc(PHY_MEM_SIZE, sizeof(char));
    VirMem = (char*)calloc(virtual_mem_size, sizeof(char));

    if (!PhysMem || !VirMem)
    {
        printf("fail allocation of Phys/Virt !!! \n");
        return;
    }

    printf("Phys base addr=%p Virtual base addr=%p \n", PhysMem, VirMem);

    // Init the PT: all are out of RAM + only part are allowed
    for (int i = 0; i < PT_num_entries; i++)
    {
        PT_table[i].in_ram = 0;
        if (((i >= 0) && (i < 24)) || (i > 56))
            PT_table[i].not_allowed = 0;
        else
            PT_table[i].not_allowed = 1;
    }

    for (int i = 0; i < NumFrames; i++)
    {
        FrameInfo[i].busy = 0;
    }

    // Init the PT 
    FrameInfo[0].busy = 1;            // first Frame for the PT

    /*
        Test scenarios:
                Scenario A:
                        - read the byte at offset 1 of the page 1
                        - write byte of  value 15 to offset 1 of the page 1
                        - read the byte at offset 1 of the page 1
                Scenario B:
                        - a loop of Scenario A: for all the pages
    */
    printf("======================= \n");
    printf("=====  START TESTS ==== \n");
    printf("======================= \n");
    printPageTable(1);

    // USE scenario A  (for acenario B set this to 0) 
    int scenario_A = 1;
    char write_val = 15;

    if (scenario_A)
    {
        test_single_write_read(1, 1, write_val);
    }
    else
    {
        // Write and Read for each Page at offset 1 ( this will bring pages as long there is a free frame
        for (int i = 0; i < PT_num_entries; i++)
        {
            test_single_write_read(i, 1, write_val);
        }
    }

    printPageTable(1);
}