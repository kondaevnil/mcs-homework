#include "str.h"

char * my_strcpy(char * restrict s1, const char * restrict s2)
{
    char *tmp = s1;

    while (*s2 != '\0')
        *s1++ = *s2++;

    *s1 = *s2;

    return tmp;
}

char * my_strcat(char * restrict s1, const char * restrict s2)
{
    char *tmp = s1;

    while (*s1 != '\0')
        s1++;

    while (*s2 != '\0')
        *s1++ = *s2++;

    *s1 = *s2;

    return tmp;
}

int my_strcmp(const char *s1, const char *s2)
{
    while (*s1 != '\0' && *s1 == *s2)
        s1++, s2++;

    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

size_t my_strlen(const char *s)
{
    const char *tmp = s;

    while (*s++ != '\0') ;

    return s - tmp - 1;
}
