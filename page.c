#include "pcb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "page.h"
#include "shellmemory.h"
#include "ready_queue.h"

int set_next_page(PCB pcb, int page_number)
{
    char *lines[3];
    FILE *script = fopen(pcb->filename, "r");
    if (script == NULL)
        return -1;
    int lineCounter = 0;
    char line[101];
    int counter = 0;
    int mylines = 0;

    for (; fgets(line, sizeof(line), script) != NULL; lineCounter++)
    {
        if (lineCounter < (page_number + 1) * 3 && lineCounter >= page_number * 3)
        {
            lines[lineCounter % 3] = strdup(line);
            counter++;
            mylines++;
        }
        memset(line, 0, sizeof(line));
    }

    int mydebugtest;
    for (mylines; mylines > 0; mylines--)
    {
        // printf("current: %s\n", line[mylines]);
        mydebugtest++;
    }
    // printf("debug: %d\n", mydebugtest);

    if (counter == 0)
        return 0;
    lines_space(counter, lines);

    int free_spot = mem_get_spot();
    if (free_spot != -1)
    {
        int set_spot = free_spot / 3;
        mem_set_page(free_spot, pcb, lines);
        pcb_page_set(pcb, set_spot, page_number);
    }
    else
    {
        page_fault(pcb, page_number, lines);
    }
    fclose(script);
    return 0;

    for (int i = 0; i < mylines; i++)
    {
        free(lines[i]);
    }
}

void pcb_page_set(PCB pcb, int spot, int page_number)
{
    pcb->page_table[page_number].id = pcb->id;
    pcb->page_table[page_number].frame = spot;
    pcb->page_table[page_number].real = 1;
}

void print_page(int start_pos, PTE victim_page)
{
    for (int i = 0; i < 3; i++)
    {
        char *line = mem_get_command(i + start_pos * 3, victim_page->id);
        printf("%s", line);
    }
}

void page_fault(PCB pcb, int page_number, char **lines)
{
    printf("Page fault! Victim page contents:");
    printf("\n\n");
    pcb_LR victim_LRU_LL = get_low_rup();
    PTE victim_page = victim_LRU_LL->page_table_entry;
    int frame = victim_page->frame;
    print_page(frame, victim_page);
    printf("\nEnd of victim page contents.\n");
    victim_page->frame = -1;
    victim_page->real = -1;
    mem_set_page(frame * 3, pcb, lines);
    pcb_page_set(pcb, frame, page_number);
    remove_pcb_LR();
}
