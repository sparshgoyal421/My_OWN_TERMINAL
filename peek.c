#include "headers.h"

struct File_information
{
    char name[NAME_SIZE];
    mode_t mode;
    char type;
    long links;
    char owner[NAME_SIZE];
    char group[NAME_SIZE];
    long size;
    char modified[32];
    int flag_for_x;
};

int compareByName(const void *a, const void *b)
{
    struct File_information *A = (struct File_information *)a;
    struct File_information *B = (struct File_information *)b;
    return strcmp(A->name, B->name);
}
void printing(char *direc, int flag_for_a_flag) {
    char *copy = (char *)malloc(PATH_SIZE * sizeof(char));
    strcpy(copy, direc);
    DIR *specified = opendir(direc);

    if (specified == NULL) {
        perror("opendir");
        printf("No such file or directory\n");
    } else {
        struct dirent *thing;
        struct stat file_statistics;
        struct File_information file_info[NO_OF_FILES];
        int num = 0;

        while ((thing = readdir(specified)) != NULL) {
            if (thing->d_name[0] == '.') {
                if (flag_for_a_flag == 0) {
                    continue;
                }
            }
            strcat(direc, "/");
            strcat(direc, thing->d_name);

            if (stat(direc, &file_statistics) == 0) {
                strcpy(file_info[num].name, thing->d_name);
                char type = '-';
                if (__S_IFDIR & file_statistics.st_mode) {
                    type = 'd';
                } else if (file_statistics.st_mode & S_IXUSR || file_statistics.st_mode & S_IXGRP || file_statistics.st_mode & S_IXOTH) {
                    type = 'x';
                }
                file_info[num].type = type;
                num++;
            } else {
                perror("stat");
            }

            strcpy(direc, copy);
        }

        closedir(specified);

        qsort(file_info, num, sizeof(struct File_information), compareByName);

        for (int i = 0; i < num; i++) {
            char *colour;
            if (file_info[i].type == 'd') {
                colour = BLUE;
            } else if (file_info[i].type == '-') {
                colour = WHITE;
            } else {
                colour = GREEN;
            }
            printf("%s%s%s\n", colour, file_info[i].name, WHITE);
        }
    }
}

char *check_permissions(mode_t mode)
{
    char *temp = (char *)malloc(10 * sizeof(char));
    int counter = 0;
    if (mode & S_IRUSR)
    {
        temp[counter++] = 'r';
    }
    else
        temp[counter++] = '-';
    if (mode & S_IWUSR)
    {
        temp[counter++] = 'w';
    }
    else
        temp[counter++] = '-';
    if (mode & S_IXUSR)
    {
        temp[counter++] = 'x';
    }
    else
        temp[counter++] = '-';
    if (mode & S_IRGRP)
    {
        temp[counter++] = 'r';
    }
    else
        temp[counter++] = '-';
    if (mode & S_IWGRP)
    {
        temp[counter++] = 'w';
    }
    else
        temp[counter++] = '-';
    if (mode & S_IXGRP)
    {
        temp[counter++] = 'x';
    }
    else
        temp[counter++] = '-';
    if (mode & S_IROTH)
    {
        temp[counter++] = 'r';
    }
    else
        temp[counter++] = '-';
    if (mode & S_IWOTH)
    {
        temp[counter++] = 'w';
    }
    else
        temp[counter++] = '-';
    if (mode & S_IXOTH)
    {
        temp[counter++] = 'x';
    }
    else
        temp[counter++] = '-';
    temp[counter] = '\0';
    return temp;
}
void implimenting_l_flag(char *direc, int flag_for_a_flag) {
    char *copy = (char *)malloc(PATH_SIZE * sizeof(char));
    strcpy(copy, direc);
    DIR *specified = opendir(direc);

    if (specified == NULL) {
        perror("opendir");
        printf("Error in opening the directory\n");
    } else {
        struct dirent *thing;
        struct stat file_statistics;
        struct File_information file_info[NO_OF_FILES];
        int total_number_of_blocks = 0;
        int num = 0;

        while ((thing = readdir(specified)) != NULL) {
            if (thing->d_name[0] == '.') {
                if (flag_for_a_flag == 0) {
                    continue;
                }
            }
            strcat(direc, "/");
            strcat(direc, thing->d_name);

            if (stat(direc, &file_statistics) == 0) {
                total_number_of_blocks += (file_statistics.st_blocks / 2); // Convert block size to 1K blocks

                // Get owner and group names
                struct passwd *pw = getpwuid(file_statistics.st_uid);
                struct group *gr = getgrgid(file_statistics.st_gid);

                if (pw == NULL || gr == NULL) {
                    perror("getpwuid/getgrgid");
                    printf("Error in getting owner or group information\n");
                } else {
                    // Format modification time
                    char time_str[32];
                    strftime(time_str, sizeof(time_str), "%b %d %Y %H:%M", localtime(&file_statistics.st_mtime));

                    strcpy(file_info[num].name, thing->d_name);

                    char type = '-';
                    file_info[num].flag_for_x = 0;
                    if (S_ISDIR(file_statistics.st_mode)) {
                        type = 'd';
                    } else {
                        type = '-';
                        if ((file_statistics.st_mode & S_IXUSR) || (file_statistics.st_mode & S_IXGRP) || (file_statistics.st_mode & S_IXOTH)) {
                            file_info[num].flag_for_x = 1;
                        }
                    }

                    file_info[num].mode = file_statistics.st_mode;
                    file_info[num].type = type;
                    file_info[num].links = (long)file_statistics.st_nlink;
                    strcpy(file_info[num].owner, pw->pw_name);
                    strcpy(file_info[num].group, gr->gr_name);
                    file_info[num].size = (long)file_statistics.st_size;
                    strcpy(file_info[num].modified, time_str);
                    num++;

                    strcpy(direc, copy);
                }
            } else {
                perror("stat");
                printf("Error in getting the file information\n");
            }
        }

        closedir(specified);

        qsort(file_info, num, sizeof(struct File_information), compareByName);

        printf("total %d\n", total_number_of_blocks);

        for (int i = 0; i < num; i++) {
            char *colour;
            if (file_info[i].type == 'd') {
                colour = BLUE;
            } else if (file_info[i].type == '-' && file_info[i].flag_for_x == 0) {
                colour = WHITE;
            } else if (file_info[i].type == '-' && file_info[i].flag_for_x == 1) {
                colour = GREEN;
            }

            printf("%c%s %4ld %s %s %8ld %s %s%s%s\n",
                   file_info[i].type,
                   check_permissions(file_info[i].mode),
                   file_info[i].links,
                   file_info[i].owner,
                   file_info[i].group,
                   file_info[i].size,
                   file_info[i].modified,
                   colour,
                   file_info[i].name,
                   WHITE);
        }
    }
}

void implimenting_peek(char *command, char *home_for_shell)
{
    char *temp = (char *)malloc(PATH_SIZE * sizeof(char));
    int i = 0;
    char *token = strtok(command, " \n");
    int flag_for_a_flag = 0;
    int flag_for_l_flag = 0;

    while (token != NULL)
    {

        if (i == 0)
        {
            token = strtok(NULL, " \n");
            i++;
            continue;
        }

        if (strcmp(token, "-a") == 0)
        {
            flag_for_a_flag = 1;
        }
        else if (strcmp(token, "-l") == 0)
        {
            flag_for_l_flag = 1;
        }
        else if (strcmp(token, "-al") == 0)
        {
            flag_for_a_flag = 1;
            flag_for_l_flag = 1;
        }
        else if (strcmp(token, "-la") == 0)
        {
            flag_for_a_flag = 1;
            flag_for_l_flag = 1;
        }
        else
        {
            int p = strlen(token);
            int flag = 0;

            for (int l = 0; l < p; l++)
            {
                if (token[l] == '/')
                    flag = 1;
            }

            if (flag)
            {
                if (token[0] == '~')
                {
                    strcpy(temp, home_for_shell);
                    int o = strlen(temp);
                    for (int m = o; m < o + p - 1; m++)
                    {
                        temp[m] = token[m - o + 1];
                    }
                    if (flag_for_l_flag == 0)
                        printing(temp, flag_for_a_flag);
                    else
                        implimenting_l_flag(temp, flag_for_a_flag);
                }
                else
                {
                    strcpy(temp, token);
                    if (flag_for_l_flag == 0)
                        printing(temp, flag_for_a_flag);
                    else
                        implimenting_l_flag(temp, flag_for_a_flag);
                }
            }
            else
            {
                if (strcmp(token, "..") == 0)
                {
                    if (getcwd(temp, PATH_SIZE) == NULL)
                    {
                        perror("getcwd");
                    }

                    char *token1 = strtok(temp, "/");
                    char **tokens = (char **)malloc(100 * sizeof(char *));
                    int j = 0;
                    while (token1 != NULL)
                    {
                        tokens[j] = (char *)malloc(NAME_SIZE * sizeof(char));
                        strcpy(tokens[j++], token1);
                        token1 = strtok(NULL, "/");
                    }

                    char *back = (char *)malloc(PATH_SIZE * sizeof(char));

                    if (j == 1 || j == 0)
                        strcat(back, "/");
                    else
                    {
                        for (int k = 0; k < j - 1; k++)
                        {
                            strcat(back, "/");
                            strcat(back, tokens[k]);
                        }
                    }

                    if (flag_for_l_flag == 0)
                        printing(back, flag_for_a_flag);
                    else
                        implimenting_l_flag(back, flag_for_a_flag);
                }
                else if (strcmp(token, ".") == 0)
                {
                    if (getcwd(temp, PATH_SIZE) == NULL)
                        perror("getcwd");
                    if (flag_for_l_flag == 0)
                        printing(temp, flag_for_a_flag);
                    else
                        implimenting_l_flag(temp, flag_for_a_flag);
                }
                else if (strcmp(token, "~") == 0)
                {
                    if (getcwd(temp, PATH_SIZE) == NULL)
                        perror("getcwd");
                    if (flag_for_l_flag == 0)
                        printing(home_for_shell, flag_for_a_flag);
                    else
                        implimenting_l_flag(home_for_shell, flag_for_a_flag);
                }
                else if (strcmp(token, "-") == 0)
                {
                    if (flag_for_l_flag == 0)
                        printing(last_directory, flag_for_a_flag);
                    else
                        implimenting_l_flag(last_directory, flag_for_a_flag);
                }
                else
                {
                    if (getcwd(temp, PATH_SIZE) == NULL)
                        perror("getcwd");
                    if (strcmp(temp, "/") == 0)
                        strcat(temp, token);
                    else
                    {
                        strcat(temp, "/");
                        strcat(temp, token);
                    }

                    if (flag_for_l_flag == 0)
                        printing(temp, flag_for_a_flag);
                    else
                        implimenting_l_flag(temp, flag_for_a_flag);
                }
            }
        }
        token = strtok(NULL, " \n");
        i++;
    }

    if (i == 1 || (i == 2 && flag_for_a_flag == 1) || (i == 2 && flag_for_l_flag == 1))
    {
        if (getcwd(temp, PATH_SIZE) == NULL)
            perror("getcwd");
        if (flag_for_l_flag == 0)
            printing(temp, flag_for_a_flag);
        else
            implimenting_l_flag(temp, flag_for_a_flag);
    }
}