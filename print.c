#include "print.h"

void print_str(const char *str, int max_len, int leading_space, int trailing_space)
{
    if (strlen(str) == 0 || max_len <= 0)
        return;

    while (leading_space-- > 0)
        printf(" ");

    if (strlen(str) > max_len) {
        while (max_len-- > 0) {
            printf("%c",*str++);
        }
    } else {
        printf("%s", str);
        max_len = max_len - strlen(str);
        while (max_len-- > 0) {
            printf(" ");
        }
    }

    while (trailing_space-- > 0)
        printf(" ");
}

void print_ch(char c, int max_len)
{
    if (max_len <= 0)
        return;

    while (max_len-- > 0)
        printf("%c", c);
}