#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ready_queue.h"
#include "pcb.h"
#include "shellmemory.h"
#include "shell.h"
#include "interpreter.h"
#include "page.h"

//reference: stackoverflow, chatgpt, geeksforgeeks
const int MAX_ARGS_SIZE = 7;

int help();
int quit();
int set(char *var, char *value[], int words);
int print(char *var);
int run(char *script);
int echo(char *string[], int numberOfWords);
int my_cd(char *path);
int my_ls();
int my_mkdir(char *dirname);
int my_touch(char *filename);
int exec(char *commands[], int num_commands);

int badcommand()
{
	printf("%s\n", "Unknown Command");
	return 1;
}

int my_badcommand(char *message)
{
	printf("Bad command: %s\n", message);
	return 1;
}

int badcommandFileDoesNotExist()
{
	printf("%s\n", "Bad command: File not found");
	return 3;
}

int badcommandMkdir()
{
	printf("%s\n", "Bad command: my_mkdir");
	return 4;
}

int badcommandCd()
{
	printf("%s\n", "Bad command: my_cd");
	return 5;
}



int interpreter(char *command_args[], int args_size)
{
	int i;
	if (args_size < 1)
	{
		return badcommand();
	}

	if (args_size > MAX_ARGS_SIZE)
	{
		return badcommand();
	}

	if (strcmp(command_args[0], "help") == 0)
	{
		// help
		if (args_size != 1)
			return badcommand();
		return help();
	}
	else if (strcmp(command_args[0], "quit") == 0)
	{
		// quit
		if (args_size != 1)
			return badcommand();
		return quit();
	}
	else if (strcmp(command_args[0], "set") == 0)
	{
		// set
		if (args_size < 3)
			return badcommand();
		return set(command_args[1], command_args, args_size);
	}
	else if (strcmp(command_args[0], "print") == 0)
	{
		if (args_size != 2)
			return badcommand();
		return print(command_args[1]);
	}
	else if (strcmp(command_args[0], "run") == 0)
	{
		if (args_size != 2)
			return badcommand();
		return run(command_args[1]);
	}
	else if (strcmp(command_args[0], "echo") == 0)
	{
		if (args_size < 2)
			return badcommand();
		return echo(command_args, args_size);
	}
	else if (strcmp(command_args[0], "my_cd") == 0)
	{
		if (args_size != 2)
			return badcommand();
		return my_cd(command_args[1]);
	}
	else if (strcmp(command_args[0], "my_ls") == 0)
	{
		if (args_size != 1)
			return badcommand();
		return my_ls();
	}
	else if (strcmp(command_args[0], "my_touch") == 0)
	{
		if (args_size != 2)
			return badcommand();
		return my_touch(command_args[1]);
	}
	else if (strcmp(command_args[0], "my_mkdir") == 0)
	{
		if (args_size != 2)
			return badcommand();
		return my_mkdir(command_args[1]);
	}
	else if (strcmp(command_args[0], "exec") == 0)
	{
		if (args_size < 3)
			return my_badcommand("[exec] requires at least 2 arguments\n\t exec <filename> <filename> ... <policy>");
		return exec(command_args, args_size);
	}
	else
		return badcommand();
}

int help()
{
	char help_string[] = "COMMAND			DESCRIPTION\n \
help			Displays all the commands\n \
quit			Exits / terminates the shell with “Bye!”\n \
set VAR STRING		Assigns a value to shell memory\n \
print VAR		Displays the STRING assigned to VAR\n \
run SCRIPT.TXT		Executes the file SCRIPT.TXT\n ";
	printf("%s\n", help_string);
	return 0;
}

int quit()
{
	printf("%s\n", "Bye!");
	exit(0);
}

int set(char *var, char *value[], int length)
{
	char variable[100];
	strcpy(variable, var);
	/* To ensure the variable is unique for each process, concat the process ID to the variable name,
	this is just in case 2 different processes share a variable.*/
	PCB pcb = get_currently_running_process();
	if (pcb != NULL) pcb->curr_line++;

	char *separator = " ";
	char buffer[100];
	char *link = "=";
	for (int i = 0; i < 100; i++) buffer[i] = '\0';
	for (int i = 2; i < length; i++)
	{
		strcat(buffer, value[i]);
		if (i != (length - 1)) strcat(buffer, separator);
	}
	if (pcb != NULL) strcat(variable, pcb->id);

	int spot = mem_set_value(variable, buffer);

	if (spot == -1) printf("%s\n", "Memory is full");
	return 0;
}

int print(char *var)
{
	char process_var[100];
	strcpy(process_var, var);

	PCB pcb = get_currently_running_process();
	if (pcb != NULL) pcb->curr_line++;
	// same as above
	if (pcb != NULL) strcat(process_var, pcb->id);
	char *value = mem_get_value(process_var);
	printf("%s\n", value);
	return 0;
}

int echo(char *string[], int length)
{
	PCB pcb = get_currently_running_process(); 
	if (pcb != NULL) pcb->curr_line++;

	// Loop over each word in the input array
	for (int i = 1; i < length; i++)
	{
		char *word = string[i];

		// If the word starts with a '$', it is a variable name
		if (word[0] == '$')
		{
			char process_var[100];
			char *var = word + 1;
			PCB pcb = get_currently_running_process(); 
			if (pcb != NULL) // same as above
			{
				strcpy(process_var, var);
				strcat(process_var, pcb->id);
			}
			char *value = mem_get_value(process_var); 

			if (strcmp(value, "Variable does not exist") != 0) printf("%s", value);
		}
		else 
		{
			printf("%s", string[i]);
		}
	}

	printf("\n"); 
	return 0; 
}



int my_ls()
{
	return 0;
}

int my_cd(char *path)
{
	return 0;
}

int my_touch(char *path)
{
	return 0;
}

int my_mkdir(char *directoryName)
{
	return 0;
}

int run(char *script)
{
	// since there is only 1 file to be run, simplest way is hard code as RR 

	char *commands[100];

	commands[0] = "exec";
	commands[1] = script;
	commands[2] = "RR";

	int num_commands = 3;
	exec(commands, num_commands);
	return 0;
}

/*
	only uses round robin policy
*/
int exec(char *commands[], int num_commands)
{
    int errCode = 0;
    char *policy = "RR";
    struct PCBStruct pcbs[num_commands - 2]; // Create an array of PCBs to hold the program data for each command

    // Loop over each command in the input array
    for (int i = 1; i < num_commands - 1; i++)
    {
        char fileName[100];
        char *script = commands[i];
        FILE *p = fopen(script, "rt"); 

        if (p == NULL) return badcommandFileDoesNotExist();

        strcpy(fileName, "BackingStore/");
        strcat(fileName, script);

        FILE *f = fopen(fileName, "w"); // Open the output file for writing

        // If the newly made file in BackingStore cannot be opened, return an error code
        if (f == NULL) return my_badcommand("Failed to open your file in BackingStore ");

        char line[1000];
        int counter = 0;

		int count = 0;

		// written with help from chatgpt
        for (; fgets(line, sizeof(line), p) != NULL; counter++) {
            if (strchr(line, ';') != NULL) {
                char *token = strtok(line, ";");
                fprintf(f, "%s\n", token);
                token = strtok(NULL, ";");
                token = token + 1;
                fprintf(f, "%s", token);
                counter++;
				count++;
                continue;
            }
            fprintf(f, "%s", line);
        }
		int loop_finish = 1;
		//printf("count is: %d\n", count);
		//printf("loop_finish is: %d\n", loop_finish);

        PCB pcb = create_pcb(f, fileName); // Create a new PCB for the program data

        fclose(f);
        fclose(p); 

        pcb->num_lines = counter; 

        pcbs[i - 1] = *pcb; 
    }


    // Loop over each page in the backing store, setting the page for each PCB
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < num_commands - 2; j++)
        {
            int success = set_next_page(&pcbs[j], i);
            if (success == 1) return my_badcommand("Paging fail");
        }
    }

    // Add each PCB to the queue
    for (int i = 0; i < num_commands - 2; i++)
    {
        add_PCB(&pcbs[i]);
    }

    errCode = run_queue(policy); // only runs roundrobin.
    return errCode;
}
