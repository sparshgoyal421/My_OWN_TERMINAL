#include "headers.h"

char last_directory[PATH_SIZE];
char directory_storage_for_e_flag_in_seek[PATH_SIZE];
Node *Background_list = NULL;
int my_time;
int child_process_id;
int tokens_for_pipes_counter;
int index_for_list;
int count_for_e_in_seek;
int file_descript_for_changing_stdin;
char *my_command;
char *back_command[32680];
char tokens_for_pipes[NO_OF_COMMANDS][COMMAND_SIZE];
processes_for_activities list[1024];

void Handling_the_background_process(int signum)
{
    int status;
    pid_t completedPid;

    while ((completedPid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        Node *newNode = (Node *)malloc(sizeof(Node));
        if (newNode == NULL)
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }

        newNode->process_id = completedPid;
        newNode->process_status = status;
        newNode->Next = Background_list;
        Background_list = newNode;
    }
}

void sigint_handler(int signum)
{
    if (kill(child_process_id, 0) == 0)
        kill(child_process_id, 9);
    printf("\n");
}

void signal_for_ctrlz(int signum)
{
    if (kill(child_process_id, 0) == 0)
    {
        kill(child_process_id, SIGTSTP);
    }
    printf("\n");
}

int main()
{
    if (signal(SIGINT, sigint_handler) == SIG_ERR)
    {
        perror("Error setting up signal handler in child");
        return 1;
    }
    if (signal(SIGTSTP, signal_for_ctrlz) == SIG_ERR)
    {
        perror("Error setting up signal handler in child");
        return 1;
    }
    index_for_list = 0;
    int flag_for_exit = 0;
    char *home_for_shell = (char *)malloc(PATH_SIZE * sizeof(char));
    for (int i = 0; i < 32680; i++)
    {
        back_command[i] = (char *)malloc(COMMAND_SIZE * sizeof(char));
    }

    if (getcwd(home_for_shell, PATH_SIZE) == NULL)
        perror("getcwd");
    strcpy(last_directory, home_for_shell);
    char *copy_home = (char *)malloc(sizeof(char) * COMMAND_SIZE);
    strcpy(copy_home, home_for_shell);
    strcat(copy_home, "/record.txt");
    FILE *comm = fopen(copy_home, "r");
    if (comm == NULL)
    {
        printf("Error in file opening\n");
        return 0;
    }

    Que Q = init_queue();
    char *buffer = (char *)malloc(sizeof(char) * COMMAND_SIZE);
    while (fgets(buffer, COMMAND_SIZE, comm) != NULL)
    {
        enque(buffer, &Q);
    }
    fclose(comm);
    my_time = 0;
    my_command = (char *)malloc(NAME_SIZE * sizeof(char));

    while (1)
    {
        char *input = (char *)malloc(sizeof(char) * 4096);

        strcpy(input, input_checking(home_for_shell));

        if (strcmp(input, "sparsh") == 0)
            continue;
        char input2[4096];
        strcpy(input2, input);
        Node *current = Background_list;
        while (current != NULL)
        {
            if (WIFEXITED(current->process_status))
            {
                printf("%s exited normally (%d)\n", back_command[current->process_id], current->process_id);
            }
            else if (WIFSIGNALED(current->process_status))
            {
                printf("%s exited abnormally (%d)\n", back_command[current->process_id], current->process_id);
            }
            Node *temp = current;
            current = current->Next;
            free(temp);
        }
        Background_list = NULL;

        char **commands = (char **)malloc(NO_OF_COMMANDS * sizeof(char *));
        int counter = 0;
        char *token = strtok(input, "\n;&");
        while (token != NULL)
        {
            commands[counter] = (char *)malloc(COMMAND_SIZE * sizeof(char));
            strcpy(commands[counter++], token);
            token = strtok(NULL, "\n;&");
        }

        int x = 0;

        int coun = 0;
        int sum = 0;
        int flag_for_pipe = 0;
        int flag_for_ioredirection = 0;
        int pipefd[NO_OF_COMMANDS][2];

        int original_stdout = dup(STDOUT_FILENO);
        int original_stdin = dup(STDIN_FILENO);

        for (int i = 0; i < counter; i++)
        {
            int flag_for_background = 0;
            sum += strlen(commands[i]);
            if (input2[sum + x] == '&')
            {
                flag_for_background = 1;
            }
            x++;

            char *r9 = strstr(commands[i], "|");
            if (r9 != NULL)
            {
                coun = 0;
                flag_for_pipe = 0;
                flag_for_ioredirection = 0;
                int check = implimenting_pipes(commands[i]);
                if (check == -3)
                    continue;
                flag_for_pipe = 1;
            }
            if (flag_for_pipe == 1 && coun < tokens_for_pipes_counter)
            {
                strcpy(commands[i], tokens_for_pipes[coun]);

                pipe(pipefd[coun]);

                if (coun != 0)
                {
                    dup2(pipefd[coun - 1][0], STDIN_FILENO);
                }

                if (coun != tokens_for_pipes_counter - 1)
                {
                    dup2(pipefd[coun][1], STDOUT_FILENO);
                }
            }
            // executing the command
            char *copy_of_command = (char *)malloc(sizeof(char) * COMMAND_SIZE);
            strcpy(copy_of_command, commands[i]);
            char *tok = strtok(copy_of_command, " \t\n");
            char *r = strstr(tok, "exit");
            char *r1 = strstr(tok, "warp");
            char *r2 = strstr(tok, "peek");
            char *r3 = strstr(tok, "pastevents");
            char *r4 = strstr(tok, "proclore");
            char *r5 = strstr(tok, "seek");
            char *r6 = strstr(commands[i], ">");
            char *r7 = strstr(commands[i], "<");
            char *r8 = strstr(commands[i], ">>");
            char *r10 = strstr(tok, "iMan");
            char *r11 = strstr(tok, "activities");
            char *r12 = strstr(tok, "ping");
            char *r13 = strstr(tok, "fg");
            char *r14 = strstr(tok, "bg");
            char *r15 = strstr(tok, "neonate");
            if (r6 != NULL || r7 != NULL || r8 != NULL)
            {
                int file_descript = implimenting_io(commands[i]);
                char *to = strtok(commands[i], "<>\n");
                strcpy(commands[i], to);
            }

            char *next_command = implimenting_pastevents(input2, commands[i], &Q, copy_home);
            if (next_command != NULL)
            {
                strcpy(commands[i], next_command);
                i--;
            }

            if (r != NULL)
            {
                flag_for_exit = 1;
                break;
            }
            else if (r5 != NULL)
            {
                implimenting_seek(commands[i]);
            }
            else if (r1 != NULL)
            {
                implimenting_warp(commands[i], home_for_shell);
            }
            else if (r2 != NULL)
            {
                implimenting_peek(commands[i], home_for_shell);
            }
            else if (r4 != NULL)
            {
                implimenting_proclore(commands[i], home_for_shell);
            }
            else if (r11 != NULL)
            {
                implimenting_activities(index_for_list);
            }
            else if (r12 != NULL)
            {
                implimenting_signals(commands[i]);
            }
            else if (r13 != NULL || r14 != NULL)
            {
                implimenting_fgbg(commands[i]);
            }
            else if (r15 != NULL)
            {
                implimenting_neonate(commands[i]);
            }
            else if (r10 != NULL)
            {
                implimenting_iman(commands[i]);
            }
            else if (r3 == NULL)
            {
                time_t start_time, end_time;
                start_time = time(NULL);

                pid_t fork_return = fork();
                int status;
                char *copy = (char *)malloc(COMMAND_SIZE * sizeof(char));
                strcpy(copy, commands[i]);
                char **args = (char **)malloc((NO_OF_COMMANDS + 1) * sizeof(char *));
                char *t = strtok(commands[i], " \t\n");
                int j = 0;
                while (t != NULL)
                {
                    args[j] = (char *)malloc(COMMAND_SIZE * sizeof(char));
                    strcpy(args[j++], t);
                    t = strtok(NULL, " \t\n");
                }
                args[j] = NULL;

                if (fork_return == 0)
                {
                    child_process_id = getpid();

                    if (flag_for_background == 1)
                    {
                        setpgid(0, 0);
                    }
                    if (execvp(args[0], args) == -1)
                    {
                        printf("ERROR : '%s' is not a valid command\n", args[0]), exit(1);
                    }
                }
                else
                {
                    child_process_id = fork_return;

                    strcpy(list[index_for_list].process_name, copy);
                    list[index_for_list].process_id = child_process_id;
                    ++index_for_list;

                    int status;
                    if (!flag_for_background)
                        waitpid(fork_return, &status, WUNTRACED);
                    strcpy(back_command[fork_return], args[0]);
                    signal(SIGCHLD, Handling_the_background_process);

                    end_time = time(NULL);

                    int duration = (int)(end_time - start_time);

                    if (duration > 2 && !flag_for_background)
                    {
                        char *t = strtok(commands[i], " ");
                        my_time = duration;
                        strcpy(my_command, t);
                    }
                    else if (flag_for_background)
                        printf("%d\n", child_process_id);
                }
            }
            // execution ends here

            if (flag_for_pipe == 1 && coun != 0)
            {
                close(pipefd[coun - 1][0]);

                if (dup2(original_stdin, 0) == -1)
                {
                    perror("Error restoring original stdin");
                    exit(1);
                }
            }
            if (flag_for_pipe == 1 && coun < tokens_for_pipes_counter - 1)
            {
                close(pipefd[coun][1]);
                i--;
            }

            if (coun < tokens_for_pipes_counter)
            {
                if (dup2(original_stdout, 1) == -1)
                {
                    perror("Error restoring original stdout");
                    exit(1);
                }
            }

            if (coun == tokens_for_pipes_counter)
            {
                if (dup2(original_stdout, 1) == -1)
                {
                    perror("Error restoring original stdout");
                    exit(1);
                }
                if (dup2(original_stdin, 0) == -1)
                {
                    perror("Error restoring original stdin");
                    exit(1);
                }
                flag_for_pipe = 0;
            }

            coun++;
        }
        if (flag_for_pipe == 1)
        {
            close(pipefd[coun][1]);
        }
        if (dup2(original_stdout, 1) == -1)
        {
            perror("Error restoring original stdout");
            exit(1);
        }
        if (dup2(original_stdin, 0) == -1)
        {
            perror("Error restoring original stdin");
            exit(1);
        }
        FILE *comm = fopen(copy_home, "w");
        for (int i = 0; i < Q->size; i++)
        {
            char *x = (char *)malloc(COMMAND_SIZE * sizeof(char));
            strcpy(x, deque(&Q));
            x[strlen(x)] = '\0';
            fprintf(comm, "%s", x);
            enque(x, &Q);
            free(x);
        }
        fclose(comm);
        if (flag_for_exit == 1)
        {
            exit(0);
        }
    }
    return 0;
}
