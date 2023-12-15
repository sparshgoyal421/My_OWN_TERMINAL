#include "headers.h"

void implimenting_e_flag_with_f(char *search_path, char *target_name, char *root)
{
    DIR *specified = opendir(search_path);
    if (specified == NULL)
    {
        perror("opendir");
        return;
    }

    struct dirent *thing;
    while ((thing = readdir(specified)) != NULL)
    {
        char *copy = (char *)malloc(NAME_SIZE * sizeof(char));
        strcpy(copy, thing->d_name);
        char *base_name = strtok(copy, ".");

        if (base_name != NULL && strcmp(base_name, target_name) == 0)
        {
            if (thing->d_type != DT_DIR && strcmp(thing->d_name, ".") != 0 && strcmp(thing->d_name, "..") != 0)
            {
                strcpy(directory_storage_for_e_flag_in_seek, search_path);
                strcat(directory_storage_for_e_flag_in_seek, "/");
                strcat(directory_storage_for_e_flag_in_seek, thing->d_name);
                count_for_e_in_seek = count_for_e_in_seek + 1;
            }
            if (thing->d_type == DT_DIR && strcmp(thing->d_name, ".") != 0 && strcmp(thing->d_name, "..") != 0)
                count_for_e_in_seek++;
        }

        if (thing->d_type == DT_DIR && strcmp(thing->d_name, ".") != 0 && strcmp(thing->d_name, "..") != 0)
        {
            char *search_path_1 = (char *)malloc(PATH_SIZE * sizeof(char));
            sprintf(search_path_1, "%s/%s", search_path, thing->d_name);
            implimenting_e_flag_with_f(search_path_1, target_name, root);
        }
    }

    closedir(specified);
}

void implimenting_e_flag_with_d(char *search_path, char *target_name, char *root)
{
    DIR *specified = opendir(search_path);
    if (specified == NULL)
    {
        perror("opendir");
        return;
    }

    struct dirent *thing;
    while ((thing = readdir(specified)) != NULL)
    {
        char *copy = (char *)malloc(NAME_SIZE * sizeof(char));
        strcpy(copy, thing->d_name);
        char *base_name = strtok(copy, ".");

        if (base_name != NULL && strcmp(base_name, target_name) == 0)
        {
            if (thing->d_type == DT_DIR && strcmp(thing->d_name, ".") != 0 && strcmp(thing->d_name, "..") != 0)
            {
                strcpy(directory_storage_for_e_flag_in_seek, search_path);
                strcat(directory_storage_for_e_flag_in_seek, "/");
                strcat(directory_storage_for_e_flag_in_seek, thing->d_name);
                count_for_e_in_seek = count_for_e_in_seek + 1;
            }
            if (thing->d_type != DT_DIR && strcmp(thing->d_name, ".") != 0 && strcmp(thing->d_name, "..") != 0)
                count_for_e_in_seek++;
        }

        if (thing->d_type == DT_DIR && strcmp(thing->d_name, ".") != 0 && strcmp(thing->d_name, "..") != 0)
        {
            char *search_path_1 = (char *)malloc(PATH_SIZE * sizeof(char));
            sprintf(search_path_1, "%s/%s", search_path, thing->d_name);
            implimenting_e_flag_with_d(search_path_1, target_name, root);
        }
    }

    closedir(specified);
}

void search(char *search_path, char *target_name, char *root, int *flag, int flag_for_d, int flag_for_f)
{
    DIR *specified = opendir(search_path);
    if (specified == NULL)
    {
        perror("opendir");
        return;
    }

    struct dirent *thing;
    while ((thing = readdir(specified)) != NULL)
    {
        char *copy = (char *)malloc(NAME_SIZE * sizeof(char));
        strcpy(copy, thing->d_name);
        char *base_name = strtok(copy, ".");

        if (base_name != NULL && strcmp(base_name, target_name) == 0)
        {
            char *temp = (char *)malloc(PATH_SIZE * sizeof(char));
            strcpy(temp, ".");
            strcat(temp, search_path + strlen(root));
            if (flag_for_d == 1)
            {
                if (thing->d_type == DT_DIR && strcmp(thing->d_name, ".") != 0 && strcmp(thing->d_name, "..") != 0)
                {
                    printf("%s%s/%s%s\n", BLUE, temp, thing->d_name, WHITE);
                }
            }
            if (flag_for_f == 1)
            {
                if (thing->d_type != DT_DIR)
                {
                    printf("%s%s/%s%s\n", GREEN, temp, thing->d_name, WHITE);
                }
            }
            else if (flag_for_d == 0 && flag_for_f == 0)
            {
                if (thing->d_type == DT_DIR)
                    printf("%s%s/%s%s\n", BLUE, temp, thing->d_name, WHITE);
                else
                    printf("%s%s/%s%s\n", GREEN, temp, thing->d_name, WHITE);
            }
            *flag = 1;
        }

        if (thing->d_type == DT_DIR && strcmp(thing->d_name, ".") != 0 && strcmp(thing->d_name, "..") != 0)
        {
            char *search_path_1 = (char *)malloc(PATH_SIZE * sizeof(char));
            sprintf(search_path_1, "%s/%s", search_path, thing->d_name);
            search(search_path_1, target_name, root, flag, flag_for_d, flag_for_f);
        }
    }

    closedir(specified);
}

void implimenting_seek(char *command)
{
    int flag;
    int flag_for_d_flag = 0;
    int flag_for_f_flag = 0;
    int flag_for_e_flag = 0;
    flag = 0;
    char *temp = (char *)malloc(PATH_SIZE * sizeof(char));
    int i = 0;
    char *token = strtok(command, " \n");
    while (token != NULL)
    {
        if (i == 0)
        {
            token = strtok(NULL, " \n");
            i++;
            continue;
        }
        if (strcmp(token, "-e") == 0)
        {
            flag_for_e_flag = 1;
            token = strtok(NULL, " \n");
            i++;
            continue;
        }
        if (strcmp(token, "-d") == 0)
        {
            flag_for_d_flag = 1;
            token = strtok(NULL, " \n");
            i++;
            continue;
        }
        if (strcmp(token, "-f") == 0)
        {
            flag_for_f_flag = 1;
            token = strtok(NULL, " \n");
            i++;
            continue;
        }

        if (flag_for_d_flag == 1 && flag_for_f_flag == 1)
        {
            perror("Invalid flags!");
            return;
        }

        if ((i == 1 && flag_for_d_flag == 0 && flag_for_f_flag == 0) || (i == 2 && flag_for_d_flag == 1 && flag_for_f_flag == 0) || (i == 2 && flag_for_d_flag == 0 && flag_for_f_flag == 1) || (i == 3 && flag_for_e_flag == 1))
        {
            strcpy(temp, token);
            token = strtok(NULL, " \n");
            i++;
            continue;
        }
        if (flag_for_e_flag == 1)
        {
            if (flag_for_d_flag == 1)
            {
                count_for_e_in_seek = 0;
                implimenting_e_flag_with_d(token, temp, token);
                if (count_for_e_in_seek == 1)
                {
                    if (access(directory_storage_for_e_flag_in_seek, X_OK) == 0)
                    {
                        chdir(directory_storage_for_e_flag_in_seek);
                    }
                    else
                    {
                        perror("Missing permissions for task!");
                    }
                }
                if (count_for_e_in_seek == 0)
                    printf("No match found!\n");
            }
            else if (flag_for_f_flag == 1)
            {
                count_for_e_in_seek = 0;
                implimenting_e_flag_with_f(token, temp, token);
                if (count_for_e_in_seek == 1)
                {
                    if (access(directory_storage_for_e_flag_in_seek, R_OK) == 0)
                    {
                        FILE *f = fopen(directory_storage_for_e_flag_in_seek, "r");
                        char buffer[1000];
                        while (fgets(buffer, sizeof(buffer), f) != NULL)
                        {
                            printf("%s", buffer);
                        }
                        printf("\n");
                        fclose(f);
                    }
                    else
                    {
                        perror("Missing permissions for task!");
                    }
                }
                if (count_for_e_in_seek == 0)
                    printf("No match found!\n");
            }
            return;
        }

        search(token, temp, token, &flag, flag_for_d_flag, flag_for_f_flag);
        if (flag == 0)
            printf("No match found!\n");

        token = strtok(NULL, " \n");
        i++;
    }
    if (i == 4 && flag_for_e_flag == 1 && flag_for_d_flag == 1)
    {
        char *current_directory = (char *)malloc(PATH_SIZE * sizeof(char));
        if (current_directory == NULL)
        {
            perror("malloc");
            return;
        }
        if (getcwd(current_directory, PATH_SIZE) == NULL)
        {
            perror("getcwd");
            free(current_directory);
            return;
        }

        count_for_e_in_seek = 0;
        implimenting_e_flag_with_d(current_directory, temp, current_directory);
        if (count_for_e_in_seek == 1)
        {
            if (access(directory_storage_for_e_flag_in_seek, X_OK) == 0)
            {
                chdir(directory_storage_for_e_flag_in_seek);
            }
            else
                perror("access");
        }
        if (count_for_e_in_seek == 0)
            printf("No match found!\n");

        free(current_directory);
    }
    else if (i == 4 && flag_for_e_flag == 1 && flag_for_f_flag == 1)
    {
        char *current_directory = (char *)malloc(PATH_SIZE * sizeof(char));
        if (current_directory == NULL)
        {
            perror("malloc");
            return;
        }
        if (getcwd(current_directory, PATH_SIZE) == NULL)
        {
            perror("getcwd");
            free(current_directory);
            return;
        }

        count_for_e_in_seek = 0;
        implimenting_e_flag_with_f(current_directory, temp, current_directory);
        if (count_for_e_in_seek == 1)
        {
            if (access(directory_storage_for_e_flag_in_seek, R_OK) == 0)
            {
                FILE *f = fopen(directory_storage_for_e_flag_in_seek, "r");
                if (f == NULL)
                {
                    perror("fopen");
                    free(current_directory);
                    return;
                }

                char buffer[1000];
                while (fgets(buffer, sizeof(buffer), f) != NULL)
                {
                    printf("%s", buffer);
                }
                printf("\n");
                fclose(f);
            }
            else
                perror("access");

            free(current_directory);
        }
        if (count_for_e_in_seek == 0)
            printf("No match found!\n");
    }
    else if ((i == 2 && flag_for_d_flag == 0 && flag_for_f_flag == 0) || (i == 3 && flag_for_d_flag == 1 && flag_for_f_flag == 0) || (i == 3 && flag_for_d_flag == 0 && flag_for_f_flag == 1))
    {
        char *current_directory = (char *)malloc(PATH_SIZE * sizeof(char));
        if (current_directory == NULL)
        {
            perror("malloc");
            return;
        }
        if (getcwd(current_directory, PATH_SIZE) == NULL)
        {
            perror("getcwd");
            free(current_directory);
            return;
        }

        search(current_directory, temp, current_directory, &flag, flag_for_d_flag, flag_for_f_flag);
        if (flag == 0)
            printf("No match found!\n");

        free(current_directory);
    }
}