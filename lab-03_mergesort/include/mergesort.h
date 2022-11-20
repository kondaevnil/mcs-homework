#ifndef MERGESORT_H
#define MERGESORT_H

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

int mergesort(void *array, size_t elements, size_t element_size, int (*comparator)(const void *, const void *));

#endif