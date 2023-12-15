#include "headers.h"

char *directory_to_be_shown_in_prompt(char *home_for_shell)
{
    char *current_working_directory = (char *)malloc(PATH_SIZE * sizeof(char));
    if (getcwd(current_working_directory, PATH_SIZE) == NULL)
    {
        perror("getcwd");
        free(current_working_directory);
        return NULL;
    }

    int x = strlen(current_working_directory);
    int y = strlen(home_for_shell);
    if (x < y)
    {
        return current_working_directory;
    }
    else if (x == y)
    {
        return "~";
    }
    else
    {
        char *temp = (char *)malloc(PATH_SIZE * sizeof(char));

        temp[0] = '~';
        for (int i = 0; i < x - y; i++)
        {
            temp[i + 1] = current_working_directory[y + i];
        }

        free(current_working_directory);
        return temp;
    }
}

void prompt(char *home_for_shell)
{

    char *user_name = (char *)malloc(NAME_SIZE * sizeof(char));
    char *system_name = (char *)malloc(NAME_SIZE * sizeof(char));

    if (user_name == NULL || system_name == NULL)
    {
        perror("malloc");
        exit(1);
    }

    // username
    uid_t user_id = getuid();
    struct passwd *pw = getpwuid(user_id);
    if (pw == NULL)
    {
        perror("getpwuid");
        free(user_name);
        free(system_name);
        exit(1);
    }
    strcpy(user_name, pw->pw_name);

    // systemname
    if (gethostname(system_name, NAME_SIZE) != 0)
    {
        perror("gethostname");
        free(user_name);
        free(system_name);
        exit(1);
    }
    printf("<%s%s%s@%s%s%s:%s", PURPLE, user_name, WHITE, ORANGE, system_name, WHITE, directory_to_be_shown_in_prompt(home_for_shell));
    if (my_time > 2)
    {
        printf(" %s : %d", my_command, my_time);
    }
    printf(">");
    free(user_name);
    free(system_name);
}
