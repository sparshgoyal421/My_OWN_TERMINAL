#include "headers.h"

int implimenting_pipes(char *command)
{
    char *t = strtok(command, "|\n");
    tokens_for_pipes_counter = 0;
    while (t != NULL)
    {
        strcpy(tokens_for_pipes[tokens_for_pipes_counter], t);
        tokens_for_pipes_counter++;
        t = strtok(NULL, "|\n");
    }
}