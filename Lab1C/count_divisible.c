// Program 1: how many array numbers are divisible by at least one of 2..9.
//
// Usage:  count_divisible <input_file> [output_file]
// Input file: n, then n integers. The result is printed to the screen
// and (if the second argument is given) written to the file.

#include <stdio.h>

#include "array.h"
#include "lab_io.h"

// Is the value divisible by at least one of 2..9 (0 is divisible by everything).
static int divisible_by_any_2_to_9(long long value)
{
    for (int k = 2; k <= 9; ++k)
    {
        if (value % k == 0)
            return 1;
    }
    return 0;
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
    fclose(in);
    if (arr == NULL)
        return 1;

    size_t count = 0;
    for (size_t i = 0; i < array_size(arr); ++i)
    {
        if (divisible_by_any_2_to_9(get_value(arr, i)))
            ++count;
    }
    array_delete(arr);

    printf("%zu\n", count);
    if (argc == 3)
    {
        FILE *out = fopen(argv[2], "w");
        if (out == NULL)
        {
            fprintf(stderr, "Error: cannot open output file: %s\n", argv[2]);
            return 1;
        }
        fprintf(out, "%zu\n", count);
        fclose(out);
    }
    return 0;
}
