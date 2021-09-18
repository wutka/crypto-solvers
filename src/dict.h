#ifndef _DICT_H
#define _DICT_H

int load_dawg(char *filename);
int is_word(int dawg_ptr);
int is_eol(int dawg_ptr);
int letter(int dawg_ptr);
int next(int dawg_ptr);
int dawg_start();
int match(char *word);
int longest_match(char *word);
int match_word(char *word, int offset, int dawg_ptr, int longest_match);

#endif
