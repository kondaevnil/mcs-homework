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
    size_t m = (elements / 2) * element_size, j = (elements / 2) * element_size, i = 0;


    for (size_t k = 0; k < elements * element_size; k += element_size)
    {
        if (j >= elements * element_size || (i < m && comparator(array + i, array + j) <= 0))
            copy_data(array + i, tmp + k, element_size), i += element_size;
        else
            copy_data(array + j, tmp + k, element_size), j += element_size;
    }

    for (i = 0; i < element_size * elements; i += element_size)
        copy_data(tmp + i, array + i, element_size);
}

static void merge_recursive(void *array, void *tmp, size_t elements, size_t element_size, int (*comparator)(const void *, const void *))
{
    if (elements <= 1)
        return;

    size_t shift = (elements / 2) * element_size;

    merge_recursive(array, tmp, elements / 2, element_size, comparator);
    merge_recursive(array + shift, tmp + shift, elements - elements / 2, element_size, comparator);
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