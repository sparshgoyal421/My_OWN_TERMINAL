#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H

typedef struct queue *Que;
struct queue
{
    char *arr[15];
    int front;
    int rear;
    int size;
};

int is_full(Que *Q);
int is_empty(Que *Q);
void enque(char *str, Que *Q);
char *deque(Que *Q);
Que init_queue();
char *implimenting_pastevents(char *input, char *command, Que *Q, char *copy_home);
void replaceSubstring(char *original, char *substring, char *replacement);

#endif