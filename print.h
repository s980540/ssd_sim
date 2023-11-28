#ifndef PRINT_H
#define PRINT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void print_str(const char *str, int max_len, int leading_space, int trailing_space);
extern void print_ch(char c, int max_len);

#endif // PRINT_H