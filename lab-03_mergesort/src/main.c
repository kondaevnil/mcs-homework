#include <string.h>
#include <stdio.h>
#include "mergesort.h"

static int int_comparator(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}

static int char_comparator(const void *a, const void *b)
{
    return *(char *)a - *(char *)b;
}

static int str_comparator(const void *a, const void *b)
{
    return strcmp(*((char **)a), *(char **)b);
}

static int str_to_int(const char *string)
{
    int value = 0, dec = 1, sign = 1;
    const char *start = string;

    if (*start == '-')
        sign = -1, start++;

    while (*string != 0)
        string++;

    while (string-- != start)
    {
        value += (int)(*string - '0') * dec;
        dec *= 10;
    }

    return value * sign;
}

static void display_buffer(void *array, size_t elements, size_t element_size, void (*display)(const void *))
{
    size_t end = elements * element_size;

    for (size_t i = 0; i < end; i += element_size)
    {
        display(array + i);

        printf("%c", i < end - element_size ? ' ' : '\n');
    }
}

static void int_display(const void *element)
{
    printf("%d", *(int *)element);
}

static void char_display(const void *element)
{
    printf("%c", *(char *)element);
}

static void str_display(const void *element)
{
    printf("%s", *(char **)element);
}


int main(int argc, char *argv[])
{
    int params_count = argc - 2;

    if (strcmp(argv[1], "str") == 0 && params_count > 0)
    {
        mergesort(argv + 2, params_count, sizeof(char *), str_comparator);

        display_buffer(argv + 2, params_count, sizeof(char *), str_display);
    }
    else if (strcmp(argv[1], "int") == 0 && params_count > 0)
    {
        int *buf = (int *)malloc(sizeof(int) * params_count);

        if (buf == NULL)
        {
            printf("Error: memory allocation failed.");
            return 1;
        }

        // Copy values to buffer
        for (int i = 0; i < params_count; i++)
            buf[i] = str_to_int(argv[i + 2]);

        // Check if memory allocation is successfully done and then sorting
        if (mergesort(buf, params_count, sizeof(int), int_comparator) == -1)
        {
            free(buf);

            printf("Error: memory allocation failed.");

            return 1;
        }

        display_buffer(buf, params_count, sizeof(int), int_display);

        free(buf);
    }
    else if (strcmp(argv[1], "char") == 0 && params_count > 0)
    {
        char *buf = (char *)malloc(sizeof(char) * params_count);

        if (buf == NULL)
        {
            printf("Error: memory allocation failed.");
            return 1;
        }

        // Copy values to buffer
        for (int i = 0; i < params_count; i++)
            buf[i] = *argv[i + 2];

        // Check if memory allocation is successfully done and then sorting
        if (mergesort(buf, params_count, sizeof(char), char_comparator) == -1)
        {
            free(buf);

            printf("Error: memory allocation failed.");

            return 1;
        }

        display_buffer(buf, params_count, sizeof(char), char_display);

        free(buf);
    }

    return 0;
}