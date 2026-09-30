#ifndef LAB_IO_H
#define LAB_IO_H

// Shared input/output helpers for both programs of lab 1.

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"

// Protection against absurd sizes (so we do not try to allocate terabytes).
#define MAX_ARRAY_SIZE 100000000LL

// Reads one whole integer (a word like "42" or "-7").
// Returns 1 on success; 0 if there is no number, it is not a valid integer,
// or it does not fit into long long (no silent truncation).
static inline int read_integer(FILE *in, long long *out)
{
    char word[64];
    if (fscanf(in, "%63s", word) != 1)
        return 0;

    char *end = NULL;
    errno = 0;
    long long v = strtoll(word, &end, 10);
    if (end == word || *end != '\0' || errno == ERANGE)
        return 0;
    *out = v;
    return 1;
}

// The same, but the number must also fit into intptr_t (the array element type).
static inline int read_value(FILE *in, long long *out)
{
    long long v = 0;
    if (!read_integer(in, &v))
        return 0;
    if (v < (long long)INTPTR_MIN || v > (long long)INTPTR_MAX)
        return 0;
    *out = v;
    return 1;
}

// Format: first the size n, then n integers.
// On any problem prints a message to stderr and returns NULL.
static inline Array *read_array(FILE *in)
{
    long long n = 0;
    if (!read_integer(in, &n))
    {
        fprintf(stderr, "Error: cannot read array size\n");
        return NULL;
    }
    if (n < 0)
    {
        fprintf(stderr, "Error: array size must not be negative\n");
        return NULL;
    }
    if (n > MAX_ARRAY_SIZE)
    {
        fprintf(stderr, "Error: array size is too large\n");
        return NULL;
    }

    Array *arr = array_create((size_t)n, NULL);
    if (arr == NULL)
    {
        fprintf(stderr, "Error: not enough memory\n");
        return NULL;
    }

    for (size_t i = 0; i < (size_t)n; ++i)
    {
        long long x = 0;
        if (!read_value(in, &x))
        {
            fprintf(stderr, "Error: not enough numbers in input (or invalid number)\n");
            array_delete(arr);
            return NULL;
        }
        array_set(arr, i, (Data)(intptr_t)x);
    }
    return arr;
}

// Returns the element as a signed number.
static inline long long get_value(const Array *arr, size_t i)
{
    return (long long)(intptr_t)array_get(arr, i);
}

// Elements separated by spaces, newline at the end.
static inline void print_array(FILE *out, const Array *arr)
{
    for (size_t i = 0; i < array_size(arr); ++i)
    {
        if (i != 0)
            fputc(' ', out);
        fprintf(out, "%lld", get_value(arr, i));
    }
    fputc('\n', out);
}

#endif
