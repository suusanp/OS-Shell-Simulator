#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcb.h"
#include "page.h"


struct memory_struct *shellmemory;
int mem_size = 0;
int variable_store_start = 0;

int match(char *model, char *var)
{
	int len = strlen(var), mc = 0;
	for (int i = 0; i < len; i++)
		if (*(model + i) == *(var + i))
			mc++;
	if (mc == len)
		return 1;
	else
		return 0;
}


void mem_init(int frameSize, int varSize)
{

	shellmemory = (struct memory_struct *)malloc(1000 * sizeof(struct memory_struct));

	mem_size = frameSize + varSize;
	variable_store_start = frameSize;

	int i;
	for (i = 0; i < 1000; i++)
	{
		shellmemory[i].var = "none";
		shellmemory[i].value = "none";
	}
}

int mem_set_value(char *var_in, char *value_in)
{
	int i;
	for (i = variable_store_start; i < mem_size; i++)
	{
		if (strcmp(shellmemory[i].var, var_in) == 0)
		{
			shellmemory[i].value = strdup(value_in);
			return i;
		}
	}

	for (i = variable_store_start; i < mem_size; i++)
	{
		if (strcmp(shellmemory[i].var, "none") == 0)
		{
			shellmemory[i].var = strdup(var_in);
			shellmemory[i].value = strdup(value_in);

			return i;
		}
	}
		// change to 1 for debug
	if (0)
	{
		void print_shellmemory()
		{
			for (int i = variable_store_start; i < mem_size; i++)
			{
				if (shellmemory[i].var != NULL)
				{
					printf("%s: %s\n", shellmemory[i].var, shellmemory[i].value);
				}
			}
		}
		print_shellmemory();
	}

	return -1;
}

void mem_set_command(int index, char *scriptIndicator, char *command)
{
	shellmemory[index].var = strdup(scriptIndicator);
	shellmemory[index].value = strdup(command);
}

void mem_set_page(int index_spot, PCB pcb, char *lines[])
{
	for (int i = 0; i < 3; i++)
	{
		mem_set_command(index_spot + i, pcb->id, lines[i]);
	}
}

int mem_del_command(int index)
{
	char *var = shellmemory[index].var;
	char *indicator = "pid_";

	if (strstr(var, indicator) == NULL)
	{
		return -1;
	}

	shellmemory[index].var = "none";
	shellmemory[index].value = "none";
	return 0;
}

char *mem_get_command(int index, char *scriptIndicator)
{
	char *var = shellmemory[index].var;
	if (strcmp(var, scriptIndicator) != 0)
	{
		return NULL;
	}

	return shellmemory[index].value;
}

int mem_get_spot()
{
	int free_spot = -1;
	int counter = 0;
	for (int i = 0; i < variable_store_start; i += 3)
	{
		if (strcmp(shellmemory[i].var, "none") == 0)
		{
			if (free_spot == -1)
			{
				free_spot = i;
			}
			counter++;
		}
		else
		{
			counter = 0;
			free_spot = -1;
		}
	}

	return free_spot;
}

char *mem_get_value(char *var_in)
{
	for (int i = variable_store_start; i < mem_size; i++)
	{
		if (strcmp(shellmemory[i].var, var_in) == 0)
		{
			return shellmemory[i].value;
		}
	}
	return "Variable does not exist";
}
