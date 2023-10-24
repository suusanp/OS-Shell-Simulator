#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include "interpreter.h"
#include "shellmemory.h"
#include "ready_queue.h"
#include "shell.h"
#include "page.h"

// reference: stackoverflow, chatgpt, geeksforgeeks

#if defined(X) && defined(Y)
int x = X;
int y = Y;
#else
int x = 700;
int y = 300;
#endif

struct command
{
    char *args[10];
    int args_size;
};

int MAX_USER_INPUT = 1000;
int make_backingstore();

int main(int argc, char *argv[])
{
    int errorCode = 0;
    mem_init(x, y);

    printf("%s\n\n", "Shell version 1.2 Created January 2023");
    printf("Frame Store Size = %d; Variable Store Size = %d\n", x, y);

    char prompt = '$';
    char userInput[MAX_USER_INPUT];
    int err = 0;

    for (int i = 0; i < MAX_USER_INPUT; i++)
        userInput[i] = '\0';

    make_backingstore();

    create_ready_queue();

    int terminal = isatty(0);

    while (1)
    {
        if (isatty(fileno(stdin)))
            printf("%c ", prompt);

        char *str = fgets(userInput, MAX_USER_INPUT - 1, stdin);
        if (feof(stdin))
        {
            freopen("/dev/tty", "r", stdin);
        }

        if (strlen(userInput) > 0)
        {
            errorCode = parseInput(userInput);
            if (errorCode == -1)
                exit(99); // ignore all other errors
            memset(userInput, 0, sizeof(userInput));
        }
    }
    return 0;
}

// stackoverflow
int make_backingstore()
{
    char *path = "BackingStore";
    DIR *dir = opendir(path);
    if (dir != NULL)
    {
        closedir(dir);
        delete_directory_recursively(path);
    }
    int status = mkdir(path, 0777);
    if (status != 0)
    {
        printf("Error creating directory %s\n", path);
        exit(EXIT_FAILURE);
    }
    return 0;
}

// helped by chatgpt
void delete_directory_recursively(char *path)
{
    DIR *dir = opendir(path);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        char sub_path[99999];
        sprintf(sub_path, "%s/%s", path, entry->d_name);
        struct stat statbuf;
        if (stat(sub_path, &statbuf) == -1)
        {
            printf("Error stating file %s\n", sub_path);
            continue;
        }
        if (S_ISDIR(statbuf.st_mode))
        {
            delete_directory_recursively(sub_path);
            rmdir(sub_path);
        }
        else
        {
            remove(sub_path);
        }
    }
    closedir(dir);
    rmdir(path);
}

int parseInput(char *ui)
{
    char tmp[200];
    char *words[100];
    int a = 0;
    int b;
    int w = 0; // wordID
    int errorCode;
    for (a = 0; ui[a] == ' ' && a < 1000; a++)
        ; // skip white spaces

    while (ui[a] != '\n' && ui[a] != '\0' && a < 1000 && a < strlen(ui))
    {
        while (ui[a] == ' ')
            a++;
        if (ui[a] == '\0')
            break;
        for (b = 0; ui[a] != ';' && ui[a] != '\0' && ui[a] != '\n' && ui[a] != ' ' && a < 1000; a++, b++)
            tmp[b] = ui[a];
        tmp[b] = '\0';
        if (strlen(tmp) == 0)
            continue;
        words[w] = strdup(tmp);
        if (ui[a] == ';')
        {
            w++;
            errorCode = interpreter(words, w);
            if (errorCode == -1)
                return errorCode;
            a++;
            w = 0;
            for (; ui[a] == ' ' && a < 1000; a++)
                ; // skip white spaces
            continue;
        }
        w++;
        a++;
    }
    errorCode = interpreter(words, w);
    for (int i = 0; i < w; i++)
    {
        free(words[i]);
    }
    return errorCode;
}
