#include <stdlib.h>
#include "array.h"

struct Array {
    size_t size;   // number of elements (fixed at creation)
    Data *data;    // the elements themselves
    FFree *f;      // optional function to free user pointers on delete
};

// create array
Array *array_create(size_t size, FFree f)
{
    Array *arr = malloc(sizeof(Array));
    if (arr == NULL)
        return NULL;

    arr->size = size;
    arr->f = f;
    arr->data = NULL;

    if (size > 0)
    {
        // calloc: all elements start as 0
        arr->data = calloc(size, sizeof(Data));
        if (arr->data == NULL)
        {
            free(arr);
            return NULL;
        }
    }
    return arr;
}

// delete array, free memory
void array_delete(Array *arr)
{
    if (arr == NULL)
        return;

    if (arr->f != NULL)
    {
        for (size_t i = 0; i < arr->size; ++i)
        {
            if (arr->data[i] != 0)
                arr->f((void*)arr->data[i]);
        }
    }
    free(arr->data);
    free(arr);
}

// returns specified array element
Data array_get(const Array *arr, size_t index)
{
    if (arr == NULL || index >= arr->size)
        return (Data)0;
    return arr->data[index];
}

// sets the specified array element to the value
void array_set(Array *arr, size_t index, Data value)
{
    if (arr == NULL || index >= arr->size)
        return;
    arr->data[index] = value;
}

// returns array size
size_t array_size(const Array *arr)
{
    return arr == NULL ? 0 : arr->size;
}
