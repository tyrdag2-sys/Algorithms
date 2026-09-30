// Program 2: shift array elements left or right by a given number of steps.
// Vacated cells are filled with zeros.
//
// Usage:  shift_array <input_file> [output_file]
// Input file: n, then n integers, then the direction (left/right)
// and the number of steps.

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "array.h"
#include "lab_io.h"

typedef enum { DIR_LEFT, DIR_RIGHT } Direction;

// Direction: left / right (also l / r, case does not matter).
static int read_direction(FILE *in, Direction *dir)
{
    char word[32];
    if (fscanf(in, "%31s", word) != 1)
    {
        fprintf(stderr, "Error: cannot read shift direction\n");
        return 0;
    }
    for (char *p = word; *p != '\0'; ++p)
        *p = (char)tolower((unsigned char)*p);

    if (strcmp(word, "left") == 0 || strcmp(word, "l") == 0)
    {
        *dir = DIR_LEFT;
        return 1;
    }
    if (strcmp(word, "right") == 0 || strcmp(word, "r") == 0)
    {
        *dir = DIR_RIGHT;
        return 1;
    }
    fprintf(stderr, "Error: unknown direction '%s' (use left or right)\n", word);
    return 0;
}

// Number of steps: an integer, not less than 0.
static int read_steps(FILE *in, size_t *steps)
{
    long long v = 0;
    if (!read_integer(in, &v))
    {
        fprintf(stderr, "Error: cannot read number of steps\n");
        return 0;
    }
    if (v < 0)
    {
        fprintf(stderr, "Error: number of steps must not be negative\n");
        return 0;
    }
    *steps = (size_t)v;
    return 1;
}

static void shift(Array *arr, Direction dir, size_t steps)
{
    size_t n = array_size(arr);
    if (n == 0 || steps == 0)
        return;

    if (steps >= n) // every element "falls out" of the array
    {
        for (size_t i = 0; i < n; ++i)
            array_set(arr, i, 0);
        return;
    }

    if (dir == DIR_LEFT)
    {
        for (size_t i = 0; i + steps < n; ++i)
            array_set(arr, i, array_get(arr, i + steps));
        for (size_t i = n - steps; i < n; ++i)
            array_set(arr, i, 0);
    }
    else
    {
        for (size_t i = n; i > steps; --i)
            array_set(arr, i - 1, array_get(arr, i - 1 - steps));
        for (size_t i = 0; i < steps; ++i)
            array_set(arr, i, 0);
    }
}

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3)
    {
        fprintf(stderr, "Usage: %s <input_file> [output_file]\n", argv[0]);
        return 2;
    }

    FILE *in = fopen(argv[1], "r");
    if (in == NULL)
    {
        fprintf(stderr, "Error: cannot open input file: %s\n", argv[1]);
        return 1;
    }
    Array *arr = read_array(in);
    if (arr == NULL)
    {
        fclose(in);
        return 1;
    }

    Direction dir = DIR_LEFT;
    size_t steps = 0;
    int ok = read_direction(in, &dir) && read_steps(in, &steps);
    fclose(in);
    if (!ok)
    {
        array_delete(arr);
        return 1;
    }

    shift(arr, dir, steps);

    print_array(stdout, arr);
    int result = 0;
    if (argc == 3)
    {
        FILE *out = fopen(argv[2], "w");
        if (out == NULL)
        {
            fprintf(stderr, "Error: cannot open output file: %s\n", argv[2]);
            result = 1;
        }
        else
        {
            print_array(out, arr);
            fclose(out);
        }
    }
    array_delete(arr);
    return result;
}
