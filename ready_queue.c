#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ready_queue.h"
#include "interpreter.h"
#include "shellmemory.h"
#include "shell.h"
#include "pcb.h"
#include "page.h"


// reference: stackoverflow, chatgpt, geeksforgeeks
// assisted by tutors at cs helpdesk

struct ready_queue my_queue;
int total_number_of_PCBs = 0;
PCB PCBs[3];

pcb_LR LRU_head = NULL;
pcb_LR LRU_tail = NULL;

int create_ready_queue()
{
    my_queue.head = NULL;
    my_queue.tail = NULL;
    my_queue.size = 0;
    return 0;
}

int add_PCB(PCB pcb)
{
    PCBs[total_number_of_PCBs] = pcb;
    total_number_of_PCBs++;

    if (my_queue.size == 0)
    {
        my_queue.head = pcb;
        my_queue.tail = pcb;
        LRU_tail = LRU_head;
        my_queue.size++;
        return 0;
    }
    my_queue.tail->next_pcb = pcb;
    my_queue.tail = pcb;
    my_queue.size++;

    return 0;
}

int remove_PCB(PCB pcb)
{
    if (my_queue.size == 0)
    {
        return -1;
    }

    PCB current = my_queue.head;
    PCB previous = NULL;

    while (current != NULL)
    {
        if (current == pcb)
        {
            if (my_queue.currently_running == pcb)
            {
                my_queue.currently_running = NULL;
            }

            if (current == my_queue.head)
            {
                my_queue.head = current->next_pcb;
                my_queue.size--;
                return 0;
            }

            if (current == my_queue.tail)
            {
                previous->next_pcb = NULL;
                my_queue.tail = previous;
                my_queue.size--;
                return 0;
            }
            previous->next_pcb = current->next_pcb;
            my_queue.size--;
            return 0;
        }

        previous = current;
        current = current->next_pcb;
    }
}


int execute_next_process_command()
{
    if (my_queue.size == 0)
    {
        return -1;
    }
    PCB current = my_queue.currently_running;
    if (current->curr_line < current->num_lines)
    {
        int pageNumber = current->curr_line / 3;

        int offset = current->curr_line % 3;

        PTE page = &(current->page_table[pageNumber]);

        if (page->real == -1)
        {
            int status = set_next_page(current, pageNumber);

            if (status == -1)
            {
                return -1;
            }
            if (move_currently_running_to_tail() == -1)
            {
                return -1;
            }

            my_queue.currently_running = NULL;
            return 0;
        }
        int mem_index = page->frame * 3;
        char *command = mem_get_command(mem_index + offset, current->id);
        if (command == NULL)
        {
            return -1;
        }
        pcb_LR LRU = lp(page);
        if (LRU == NULL)
        {
            add_pcb_LR(page);
        }
        else
        {
            move_page_to_last(LRU);
        }
        int errorCode = parseInput(command);
        if (errorCode == -1)
        {
            return -1;
        }
        if (current->curr_line == current->num_lines)
        {
            if (remove_PCB(current) == -1)
            {
                return -1;
            };
        }
        return 0;
    }
    return 0;
}

int run_queue(char *policy)
{

    int errorCode = 0;
    if (my_queue.size == 0)
    {
        return -1;
    }

    int quantum_counter;

    my_queue.currently_running = my_queue.head;

    for (; my_queue.size > 0;)
    {
        PCB next = my_queue.currently_running->next_pcb;

        for (int quantum_counter = 0; quantum_counter < 2 && my_queue.currently_running != NULL; quantum_counter++)
        {
            errorCode = execute_next_process_command();
            if (errorCode == -1)
            {
                return -1;
            }
        }

        if (next == NULL)
        {
            my_queue.currently_running = my_queue.head;
        }
        else
        {
            my_queue.currently_running = next;
        }
    }


    return 0;
}

pcb_LR lp(PTE page)
{

    pcb_LR current = LRU_head;

    while (current != NULL)
    {
        if (current->page_table_entry == page)
        {
            return current;
        }
        current = current->next;
    }

    return NULL;
}


int move_page_to_last(pcb_LR page_in_LRU)
{

    if (LRU_head == LRU_tail)
    {
        return 0;
    }

    if (page_in_LRU == LRU_head)
    {
        LRU_head = LRU_head->next;
        LRU_tail->next = page_in_LRU;
        LRU_tail = page_in_LRU;
        LRU_tail->next = NULL;
        return 0;
    }

    if (page_in_LRU == LRU_tail)
    {
        return 0;
    }

    pcb_LR current = LRU_head;
    pcb_LR previous = NULL;

    while (current != NULL)
    {
        if (current == page_in_LRU)
        {
            pcb_LR next = current->next;
            previous->next = next;
            current->next = NULL;
            LRU_tail->next = current;
            LRU_tail = LRU_tail->next;
            return 0;
        }

        previous = current;
        current = current->next;
    }

    return -1;
}

pcb_LR create_pcb_LR(PTE page_table)
{
    pcb_LR pcb = malloc(sizeof(struct LeastRecentPBC));
    pcb->page_table_entry = page_table;
    pcb->next = NULL;
    return pcb;
}

pcb_LR add_pcb_LR(PTE page_table)
{
    pcb_LR pcb = create_pcb_LR(page_table);

    if (LRU_head == NULL)
    {
        LRU_head = pcb;
        LRU_tail = pcb;
        return pcb;
    }

    LRU_tail->next = pcb;
    LRU_tail = pcb;
    return pcb;
}

pcb_LR remove_pcb_LR()
{
    if (LRU_head == NULL) return NULL;
    pcb_LR temp = LRU_head;
    LRU_head = LRU_head->next;
    return temp;
}

PCB get_currently_running_process()
{
    return my_queue.currently_running;
}

pcb_LR get_low_rup()
{
    return LRU_head;
}

int move_currently_running_to_tail()
{
    if (my_queue.size == 0)
    {
        return -1;
    }

    if (my_queue.size == 1)
    {
        return 0;
    }
    if (my_queue.currently_running == my_queue.head)
    {
        my_queue.head = my_queue.head->next_pcb;
        my_queue.tail->next_pcb = my_queue.currently_running;
        my_queue.tail = my_queue.tail->next_pcb;
        my_queue.tail->next_pcb = NULL;
        return 0;
    }

    if (my_queue.currently_running == my_queue.tail)
    {
        my_queue.tail = my_queue.head;
        while (my_queue.tail->next_pcb != my_queue.currently_running)
        {
            my_queue.tail = my_queue.tail->next_pcb;
        }
        my_queue.tail->next_pcb = NULL;
        my_queue.tail->next_pcb = my_queue.currently_running;
        my_queue.tail = my_queue.tail->next_pcb;
        my_queue.tail->next_pcb = NULL;
        return 0;
    }

    PCB current = my_queue.head;
    PCB previous = NULL;

    while (current != NULL)
    {
        if (current == my_queue.currently_running)
        {
            PCB next = current->next_pcb;
            previous->next_pcb = next;
            current->next_pcb = NULL;
            my_queue.tail->next_pcb = current;
            my_queue.tail = my_queue.tail->next_pcb;
            return 0;
        }

        previous = current;
        current = current->next_pcb;
    }

    return -1;

}



