#include "str.h"

char * my_strcpy(char * restrict s1, const char * restrict s2)
{
    char *tmp = s1;

    while (*s2 != '\0')
        *tmp++ = *s2++;

    *tmp = *s2;

    return s1;
}

char * my_strcat(char * restrict s1, const char * restrict s2)
{
    char *tmp = s1;

    while (*tmp != '\0')
        tmp++;

    while (*s2 != '\0')
        *tmp++ = *s2++;

    *tmp = *s2;

    return s1;
}

int my_strcmp(const char *s1, const char *s2)
{
    while (*s1 != '\0' && *s2 != '\0')
        s1++, s2++;

    return (*s1 != '\0') - (*s2 != '\0');
}

size_t my_strlen(const char *s)
{
    size_t size = 0;

    while (*s++ != '\0')
        size++;

    return size;
}
