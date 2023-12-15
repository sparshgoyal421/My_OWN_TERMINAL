#include "headers.h"

int isProcessRunning(int pid)
{
    char proc_dir[PATH_SIZE];

    sprintf(proc_dir, "/proc/%d/status", pid);

    FILE *status_file = fopen(proc_dir, "r");
    if (status_file == NULL)
    {
        return -1;
    }

    char line[1024];
    char state;
    while (fgets(line, sizeof(line), status_file) != NULL)
    {
        if (sscanf(line, "State:\t%c", &state) == 1)
        {
            fclose(status_file);

            if (state == 'T')
            {
                return 0; // means 'stopped'
            }
            else 
            {
                return 1;
            }
        }
    }

    fclose(status_file);
}

void implimenting_activities(int index)
{
    for (int i = 0; i < index; i++)
    {
        int pid = list[i].process_id;
        int result = isProcessRunning(pid);
        if (result == -1)
            continue;
        if (result == 0)
        {
            printf("%d : %s - Stopped\n", list[i].process_id, list[i].process_name);
        }
        else
        {
            printf("%d : %s - Running\n", list[i].process_id, list[i].process_name);
        }
    }
}
