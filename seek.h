#ifndef __SEEK_H
#define __SEEK_H

void implimenting_seek(char *command);
void search(char *search_path, char *target_name, char *root, int *flag, int flag_for_d, int flag_for_f);
void implimenting_e_flag_with_f(char *search_path, char *target_name, char *root);
void implimenting_e_flag_with_d(char *search_path, char *target_name, char *root);

#endif