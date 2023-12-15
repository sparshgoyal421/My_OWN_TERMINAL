#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <termios.h>
#include <ctype.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <signal.h>
#include <time.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "prompt.h"
#include "warp.h"
#include "peek.h"
#include "pastevents.h"
#include "proclore.h"
#include "seek.h"
#include "io_redirection.h"
#include "pipes.h"
#include "iMan.h"
#include "activities.h"
#include "ping.h"
#include "fgbg.h"
#include "neonate.h"

#define NAME_SIZE 1000
#define PATH_SIZE 10000
#define COMMAND_SIZE 4098
#define NO_OF_COMMANDS 100
#define NO_OF_FILES 1000
#define NAMES 1000

// ANSI escape codes for text color
#define WHITE "\x1B[0m"
#define GREEN "\x1B[32m"
#define BLUE "\x1B[34m"
#define ORANGE "\033[38;2;255;165;0m"
#define PURPLE "\x1B[35m"
#define RED "\x1B[31m"

extern char last_directory[PATH_SIZE];
extern char directory_storage_for_e_flag_in_seek[PATH_SIZE];
extern int my_time;
extern int tokens_for_pipes_counter;
extern int count_for_e_in_seek;
extern int index_for_list;
extern int child_process_id;
extern int file_descript_for_changing_stdin;
extern char *my_command;
extern char *back_command[32680];
extern char tokens_for_pipes[NO_OF_COMMANDS][COMMAND_SIZE];

typedef struct Node
{
    pid_t process_id;
    int process_status;
    struct Node *Next;
} Node;

extern Node *Background_list;

typedef struct processes_for_activities
{
    pid_t process_id;
    char process_name[1024];
} processes_for_activities;

extern processes_for_activities list[1024];
#endif