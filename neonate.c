#include "headers.h"
void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.
 * The c_lflag field is for “local flags”
 */
void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO | ISIG);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

/**
 * stdout and stdin are buffered we disable buffering on that
 * After entering in raw mode we read characters one by one
 * Up arrow keys and down arrow keys are represented by 3 byte escape codes
 * starting with ascii number 27 i.e. ESC key
 * This way we interpret arrow keys
 * Tabs are usually handled by the term, but here we are simulating tabs for the sake of simplicity
 * Backspace move the cursor one control character to the left
 * @return
 */

char *input_checking(char *home_for_shell)
{
    char *inp = malloc(sizeof(char) * 4096);
    char c;
    while (1)
    {
        setbuf(stdout, NULL);
        enableRawMode();

        prompt(home_for_shell);

        memset(inp, '\0', 4096);
        int pt = 0;
        while (read(STDIN_FILENO, &c, 1) == 1)
        {
            if (iscntrl(c))
            {
                if (c == 10)
                    break;
                else if (c == 127)
                { // backspace
                    if (pt > 0)
                    {
                        if (inp[pt - 1] == 9)
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                printf("\b");
                            }
                        }
                        inp[--pt] = '\0';
                        printf("\b \b");
                    }
                }
                else if (c == 3)
                { // ctrl c
                    printf("^C\n");
                    disableRawMode();
                    return "sparsh";
                }
                else if (c == 26)
                { // ctrl c
                    printf("^Z\n");
                    disableRawMode();
                    return "sparsh";
                }
                else if (c == 9)
                { // TAB character
                    char tmp[1024];
                    strcpy(tmp, inp);
                    pt = strlen(inp);
                    printf("%s", inp);
                }
                else if (c == 4)
                { //
                    disableRawMode();

                    for (int i = 0; i < index_for_list; i++)
                    {
                        kill(list[i].process_id,9);
                    }
                    
                    printf("\n");
                    exit(0);
                }
            }
            else
            {
                inp[pt++] = c;
                printf("%c", c);
            }
        }
        disableRawMode();
        printf("\n");
        inp[pt] = '\n';
        return inp;
    }
}

void implimenting_neonate(char *command)
{
    char *t = strtok(command, " \n\t");
    t = strtok(NULL, " \n\t");
    t = strtok(NULL, " \n\t");
    int time_interval = atoi(t);

    int pi = fork();
    if (pi == 0)
    {
        char buffer[1024];
        while (1)
        {
            char* str= "/proc/sys/kernel/ns_last_pid";
            FILE * f = fopen(str,"r");
            fgets(buffer,1024,f);
            printf("%s",buffer);
            sleep(time_interval);
        }
    }
    else
    {
        char *inp = malloc(sizeof(char) * 100);
        char c;
        while (1)
        {
            setbuf(stdout, NULL);
            enableRawMode();
            memset(inp, '\0', 100);
            int pt = 0;
            while (read(STDIN_FILENO, &c, 1) == 1)
            {
                if (iscntrl(c))
                {
                    if (c == 10)
                        break;
                    else if (c == 27)
                    {
                        char buf[3];
                        buf[2] = 0;
                        if (read(STDIN_FILENO, buf, 2) == 2)
                        { // length of escape code
                        }
                    }
                    else if (c == 127)
                    { // backspace
                        if (pt > 0)
                        {
                            if (inp[pt - 1] == 9)
                            {
                                for (int i = 0; i < 7; i++)
                                {
                                }
                            }
                            inp[--pt] = '\0';
                        }
                    }
                    else if (c == 9)
                    { // TAB character
                        inp[pt++] = c;
                        for (int i = 0; i < 8; i++)
                        { // TABS should be 8 spaces
                        }
                    }
                    else if (c == 4)
                    {
                        exit(0);
                    }
                    else
                    {
                    }
                }

                else
                {
                    if (c == 'x')
                    {
                        kill(pi, SIGKILL);
                        disableRawMode();
                        return;
                    }
                    inp[pt++] = c;
                }
            }
            disableRawMode();

        }
    }
}