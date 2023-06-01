#include <stdlib.h>

#include "mergesort.h"

static void copy_data(void *src, void *dest, size_t size)
{
    char *tmp_src = (char *)src;
    char *tmp_dest = (char *)dest;

    for(size_t i = 0; i < size; i++)
        tmp_dest[i] = tmp_src[i];
}

static void merge(void *array, void *tmp, size_t elements, size_t element_size, int (*comparator)(const void *, const void *))
{
    size_t total_bytes_size = element_size * elements;
    size_t m = (elements / 2) * element_size;
    size_t j = m;
    size_t i = 0;


    for (size_t k = 0; k < total_bytes_size; k += element_size)
    {
        if (j >= total_bytes_size || (i < m && comparator(array + i, array + j) <= 0))
            copy_data(array + i, tmp + k, element_size), i += element_size;
        else
            copy_data(array + j, tmp + k, element_size), j += element_size;
    }

    copy_data(tmp, array, total_bytes_size);
}

static void merge_recursive(void *array, void *tmp, size_t elements, size_t element_size, int (*comparator)(const void *, const void *))
{
    if (elements <= 1)
        return;

    size_t half = elements / 2;
    size_t shift = half * element_size;

    merge_recursive(array, tmp, half, element_size, comparator);
    merge_recursive(array + shift, tmp + shift, elements - half, element_size, comparator);
    merge(array, tmp, elements, element_size, comparator);
}

int mergesort(void *array, size_t elements, size_t element_size, int (*comparator)(const void *, const void *))
{
    void *tmp_array = malloc(elements * element_size);

    if (tmp_array == NULL)
        return -1;

    merge_recursive(array, tmp_array, elements, element_size, comparator);

    free(tmp_array);

    return 0;
}