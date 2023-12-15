#include "headers.h"

void implimenting_fgbg(char *command)
{
    char *t = strtok(command, " \n\t");
    if (strcmp(t, "fg") == 0)
    {
        t = strtok(NULL, " \n\t");
        int process_id = atoi(t);

        if (kill(process_id, 0) == 0)
        {
            if (kill(process_id, SIGCONT) == -1)
            {
                perror("kill");
            }

            int status;
            waitpid(process_id, &status, WUNTRACED);
        }
        else
            printf("No such process found\n");
    }
    else
    {

        t = strtok(NULL, " \n\t");
        int process_id = atoi(t);
        if (kill(process_id, 0) == 0)
        {
            if (kill(process_id, SIGCONT) == -1)
            {
                perror("kill");
            }
        }
        else
            printf("No such process found\n");
    }
}