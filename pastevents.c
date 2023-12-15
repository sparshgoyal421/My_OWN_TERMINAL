#include "headers.h"

int is_full(Que *Q)
{
    if ((*Q)->size == 15)
        return 1;
    else
        return 0;
}
int is_empty(Que *Q)
{
    if ((*Q)->size == 0)
        return 1;
    else
        return 0;
}
void enque(char *str, Que *Q)
{
    if (++(*Q)->rear == 15)
        (*Q)->rear = 0;
    (*Q)->arr[(*Q)->rear] = (char *)malloc(COMMAND_SIZE * sizeof(char));
    strcpy((*Q)->arr[(*Q)->rear], str);
    (*Q)->size++;
}
char *deque(Que *Q)
{
    if (!is_empty(Q))
    {
        char *temp = (*Q)->arr[((*Q)->front) % 15];
        (*Q)->front++;
        (*Q)->size--;
        return temp;
    }
}
Que init_queue()
{
    Que Q;
    Q = (Que)malloc(sizeof(struct queue));
    Q->front = 0;
    Q->rear = -1;
    Q->size = 0;
    return Q;
}

char *implimenting_pastevents(char *input, char *command, Que *Q, char *copy_home)
{
    char *result1 = strstr(command, "pastevents");
    char *result2 = strstr(command, "purge");
    char *result3 = strstr(command, "execute");

    if (result2 != NULL)
    {
        while ((*Q)->size != 0)
        {
            deque(Q);
        }
        (*Q)->front = 0;
        (*Q)->rear = -1;
        return NULL;
    }
    else if (result3 != NULL)
    {
        char *copy = (char *)malloc(COMMAND_SIZE * sizeof(char));
        strcpy(copy, command);
        char *t = strtok(command, " \t");
        t = strtok(NULL, " \t");
        t = strtok(NULL, " \t");

        int index = t[0] - '0';

        FILE *file = fopen(copy_home, "r");
        char *a[15];
        for (int i = 0; i < 15; i++)
        {
            a[i] = (char *)malloc(COMMAND_SIZE * sizeof(char));
        }
        int g = 0;
        while (fgets(a[g], COMMAND_SIZE, file) != NULL)
        {
            g++;
        }

        index = g - index;

        // if (strcmp(a[index], (*Q)->arr[(*Q)->rear]) != 0)
        // {
        //     enque(a[index],Q);
        // }

        return a[index];
    }
    else if (result1 != NULL)
    {
        if ((*Q)->size == 0)
            return NULL;
        for (int i = 0; i < (*Q)->size; i++)
        {
            char *temp = deque(Q);
            printf("%s", temp);
            enque(temp, Q);
        }
        return NULL;
    }

    if (is_full(Q))
        deque(Q);

    if (is_empty(Q))
    {
        if (strstr(input, "pastevents") == NULL)
            enque(input, Q);
        return NULL;
    }
    else
    {
        if (strcmp(input, (*Q)->arr[(*Q)->rear]) == 0)
        {
            return NULL;
        }
        else
        {
            if (strstr(input, "pastevents") == NULL)
                enque(input, Q);
            return NULL;
        }
    }
}
