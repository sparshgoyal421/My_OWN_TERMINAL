#ifndef __NEONATE_H
#define __NEONATE_H
void die(const char *s);
void disableRawMode();
void enableRawMode();
char *input_checking(char*home_for_shell);
void implimenting_neonate(char*command);
#endif