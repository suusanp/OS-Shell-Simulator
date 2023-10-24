
#ifndef READYQUEUE_H
#define READYQUEUE_H

#include "pcb.h"

int create_ready_queue();

int add_PCB(PCB process);

int remove_PCB(PCB process);

int execute_next_process_command();
int move_currently_running_to_tail();

pcb_LR lp(PTE page);
int run_queue(char *policy);

PCB get_currently_running_process();

pcb_LR add_pcb_LR(PTE page_table);
pcb_LR remove_pcb_LR();
pcb_LR get_low_rup();
int move_page_to_last(pcb_LR page_in_LRU);
#endif
