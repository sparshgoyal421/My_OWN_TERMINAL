#include "headers.h"

int implimenting_io(char *command)
{
    char *copy = (char *)malloc(COMMAND_SIZE * sizeof(char));
    strcpy(copy, command);
    char **token_for_io = (char **)malloc(NO_OF_COMMANDS * sizeof(char *));
    char *temp = strtok(command, "><\n");
    int j = 0;
    while (temp != NULL)
    {
        token_for_io[j] = (char *)malloc(COMMAND_SIZE * sizeof(char));
        strcpy(token_for_io[j], temp);
        j++;
        temp = strtok(NULL, "><\n");
    }
    token_for_io[j] = NULL;

    if (strstr(copy, "<") != NULL)
    {
        char *path_for_file = (char *)malloc(PATH_SIZE * sizeof(char));
        if (getcwd(path_for_file, PATH_SIZE) == NULL)
            perror("getcwd");
        strcat(path_for_file, "/");
        char *q = strtok(token_for_io[1], " \t\n");

        strcat(path_for_file, q);

        if (access(path_for_file, F_OK) == -1)
        {
            printf("No such input file found!\n");
            exit(1);
        }
        else
        {
            file_descript_for_changing_stdin = open(path_for_file, O_RDONLY);
        }

        if (dup2(file_descript_for_changing_stdin, STDIN_FILENO) == -1)
        {
            perror("Error redirecting stdout");
            exit(1);
        }
    }

    int file_descriptor;
    if (strstr(copy, ">>") != NULL)
    {
        char *path_for_file = (char *)malloc(PATH_SIZE * sizeof(char));
        if (getcwd(path_for_file, PATH_SIZE) == NULL)
            perror("getcwd");
        strcat(path_for_file, "/");

        char *q;
        if (token_for_io[2] != NULL)
        {
            q = strtok(token_for_io[2], " \t\n");
        }
        else
            q = strtok(token_for_io[1], " \t\n");

        strcat(path_for_file, q);

        if (access(path_for_file, F_OK) == -1)
        {
            file_descriptor = open(path_for_file, O_CREAT | O_WRONLY, 0644);
            if (file_descriptor == -1)
            {
                perror("Error creating file");
                exit(1);
            }
        }
        else
        {
            file_descriptor = open(path_for_file, O_APPEND | O_WRONLY);
        }

        if (dup2(file_descriptor, STDOUT_FILENO) == -1)
        {
            perror("Error redirecting stdout");
            exit(1);
        }
    }
    else if (strstr(copy, ">") != NULL)
    {
        char *path_for_file = (char *)malloc(PATH_SIZE * sizeof(char));
        if (getcwd(path_for_file, PATH_SIZE) == NULL)
            perror("getcwd");
        strcat(path_for_file, "/");

        char *q;
        if (token_for_io[2] != NULL)
        {
            q = strtok(token_for_io[2], " \t\n");
        }
        else
            q = strtok(token_for_io[1], " \t\n");

        strcat(path_for_file, q);

        if (access(path_for_file, F_OK) == -1)
        {
            file_descriptor = open(path_for_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (file_descriptor == -1)
            {
                perror("Error creating file");
                exit(1);
            }
        }
        else
        {
            file_descriptor = open(path_for_file, O_WRONLY | O_TRUNC);
        }

        if (dup2(file_descriptor, STDOUT_FILENO) == -1)
        {
            perror("Error redirecting stdout");
            exit(1);
        }
    }
    close(file_descriptor);

    return file_descriptor;
}
