#include "headers.h"

void implimenting_warp(char *command, char *home_for_shell) {
    char *temp = (char *)malloc(PATH_SIZE * sizeof(char));
    char **tokens = (char **)malloc(NO_OF_COMMANDS * sizeof(char *));
    tokens[0] = (char *)malloc(NO_OF_COMMANDS * sizeof(char));
    tokens[0] = strtok(command, " \t\n");
    int n = 0;
    while (tokens[n] != NULL) {
        tokens[++n] = (char *)malloc(NO_OF_COMMANDS * sizeof(char));
        tokens[n] = strtok(NULL, " \t\n");
    }
    int i = 1;
    for (i = 1; i < n; i++) {
        int p = strlen(tokens[i]);
        int flag = 0;

        for (int l = 0; l < p; l++) {
            if (tokens[i][l] == '/')
                flag = 1;
        }

        if (flag) {
            if (tokens[i][0] == '~') {
                strcpy(temp, home_for_shell);
                int o = strlen(temp);
                for (int m = o; m < o + p - 1; m++) {
                    temp[m] = tokens[i][m - o + 1];
                }
                getcwd(last_directory, PATH_SIZE);

                if (chdir(temp) != 0) {
                    perror("chdir");
                } else {
                    printf("%s\n", temp);
                }
            } else {
                strcpy(temp, tokens[i]);
                getcwd(last_directory, PATH_SIZE);

                if (chdir(temp) != 0) {
                    perror("chdir");
                } else {
                    printf("%s\n", temp);
                }
            }
        } else {
            if (strcmp(tokens[i], "..") == 0) {
                getcwd(temp, PATH_SIZE);
                strcpy(last_directory, temp);

                char *token1 = strtok(temp, "/");
                char **tokens_temp = (char **)malloc(100 * sizeof(char *));
                int j = 0;
                while (token1 != NULL) {
                    tokens_temp[j] = (char *)malloc(NAME_SIZE * sizeof(char));
                    strcpy(tokens_temp[j++], token1);
                    token1 = strtok(NULL, "/");
                }

                char *back = (char *)malloc(PATH_SIZE * sizeof(char));

                if (j == 1 || j == 0)
                    strcat(back, "/");
                else {
                    for (int k = 0; k < j - 1; k++) {
                        strcat(back, "/");
                        strcat(back, tokens_temp[k]);
                    }
                }

                if (chdir(back) != 0) {
                    perror("chdir");
                } else {
                    printf("%s\n", back);
                }
            } else if (strcmp(tokens[i], ".") == 0) {
                getcwd(temp, PATH_SIZE);
                strcpy(last_directory, temp);
                printf("%s\n", temp);
            } else if (strcmp(tokens[i], "~") == 0) {
                getcwd(temp, PATH_SIZE);
                strcpy(last_directory, temp);

                if (chdir(home_for_shell) != 0) {
                    perror("chdir");
                } else {
                    printf("%s\n", home_for_shell);
                }
            } else if (strcmp(tokens[i], "-") == 0) {
                if (chdir(last_directory) != 0) {
                    perror("chdir");
                } else {
                    printf("%s\n", last_directory);
                }
            } else {
                getcwd(temp, PATH_SIZE);

                if (strcmp(temp, "/") == 0)
                    strcat(temp, tokens[i]);
                else {
                    strcpy(last_directory, temp);
                    strcat(temp, "/");
                    strcat(temp, tokens[i]);
                }

                if (chdir(temp) != 0) {
                    perror("chdir");
                } else {
                    printf("%s\n", temp);
                }
            }
        }
    }

    if (i == 1) {
        getcwd(temp, PATH_SIZE);
        strcpy(last_directory, temp);

        if (chdir(home_for_shell) != 0) {
            perror("chdir");
        } else {
            printf("%s\n", home_for_shell);
        }
    }
}
