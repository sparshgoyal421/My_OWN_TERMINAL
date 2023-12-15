#include "headers.h"

void implimenting_proclore(char *command, char *home_for_shell) {
    int i = 0;
    char *token = strtok(command, " \n");
    pid_t process_id;
    while (token != NULL) {
        if (i == 0) {
            token = strtok(NULL, " \n");
            i++;
            continue;
        }
        process_id = atoi(token); // used to convert string to integer
        token = strtok(NULL, " \n");
        i++;
    }
    if (i == 1) {
        process_id = getpid();
    }
    pid_t process_group = getpgid(process_id); // process group

    char process_status;
    char *path_for_status = (char *)malloc(PATH_SIZE * sizeof(char));
    sprintf(path_for_status, "/proc/%d/stat", process_id);

    printf("pid : %d\n", process_id);

    FILE *f = fopen(path_for_status, "r");
    if (f != NULL) {
        fscanf(f, "%*d %*s %c", &process_status);
        fclose(f);
        printf("process Status : %c", process_status);
        if (process_status == 'w') {
            process_status = 'Z';
        }
        if (process_id == process_group) {
            printf("+");
        }
        printf("\n");
    } else {
        perror("Error opening stat file");
    }

    printf("Process Group: %d\n", process_group);

    char *path_for_virtual_memory = (char *)malloc(PATH_SIZE * sizeof(char));
    sprintf(path_for_virtual_memory, "/proc/%d/status", process_id);
    FILE *v = fopen(path_for_virtual_memory, "r");
    if (v != NULL) {
        char line[1000];
        while (fgets(line, sizeof(line), v)) {
            if (strstr(line, "VmSize:") != NULL) {
                char *t = strtok(line, " ");
                t = strtok(NULL, " ");
                printf("Virtual Memory: %s\n", t);
                break;
            }
        }
        fclose(v);
    } else {
        perror("Error opening status file");
    }

    char *path_for_executable = (char *)malloc(PATH_SIZE * sizeof(char));
    sprintf(path_for_executable, "/proc/%d/exe", process_id);

    char executable[1000];
    ssize_t r = readlink(path_for_executable, executable, sizeof(executable) - 1);
    if (r != -1) {
        executable[r] = '\0';
        printf("executable Path: %s\n", executable);
    } else {
        perror("Error reading executable path");
    }
}
