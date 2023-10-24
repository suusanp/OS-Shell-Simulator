
#ifndef SHELLMEMORY_H
#define SHELLMEMORY_H

void mem_init(int frameSize, int varSize);

char *mem_get_value(char *var);

int mem_set_value(char *var, char *value);

int mem_get_spot();

void mem_set_command(int index, char *scriptIndicator, char *command);

void mem_set_page(int free_spot, PCB pcb, char* lines[]);

int mem_del_command(int index);

char *mem_get_command(int index, char *scriptIndicator);

int mem_get_frameIndex(int index);

int mem_increment_age_of_frame(int frameIndex);

char *mem_get_val_at_index(int index);

char *mem_get_var_at_index(int index);

#endif
