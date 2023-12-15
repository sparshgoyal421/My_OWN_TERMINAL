#include "headers.h"

void implimenting_signals(char *command)
{
    char *t = strtok(command, " \n\t");
    t = strtok(NULL, " \n\t");
    int process_id = atoi(t);
    t = strtok(NULL, " \n\t");
    int signal_number = atoi(t);

    // Signal 0 does not actually send a signal but is used to check for process existence.
    if (kill(process_id, 0) == 0)
    {
        int signal_num = signal_number % 32;

        if (kill(process_id, signal_num) == 0)
        {
            printf("Sent signal %d to process with pid %d\n", signal_num, process_id);
        }
        else
        {
            perror("Error sending signal");
        }
    }
    else
    {
        printf("No such process found\n");
    }
}