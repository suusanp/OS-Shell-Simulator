#ifndef PAGE_H
#define PAGE_H

#include "pcb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct PageTableEntry
{
    int frame;
    int real;
    char *id;
};
typedef struct PageTableEntry *PTE;

struct Page
{
    char *lines[3];
};

struct LeastRecentPBC
{
    struct PageTableEntry *page_table_entry;
    struct LeastRecentPBC *next;
};
typedef struct LeastRecentPBC *pcb_LR;

struct PCBStruct
{
    int start_loc;
    int curr_line;
    char *id;
    struct PCBStruct *next_pcb;
    char *filename;
    int num_lines;
    struct PageTableEntry page_table[10];
};
typedef struct PCBStruct *PCB;

struct ready_queue
{
    PCB head;
    PCB tail;
    PCB currently_running;
    int size;
};

int set_next_page(PCB pcb, int page_number);
void pcb_page_set(PCB pcb, int spot, int page_number);
void init_pcb_lines(PCB pcb);
void print_page(int start_pos, PTE victim_page);
void page_fault(PCB pcb, int page_number, char** lines);

#endif