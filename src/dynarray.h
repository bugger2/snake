#ifndef DYNARRAY_H_
#define DYNARRAY_H_

#include <stdarg.h>
#include <stdlib.h>

// A standard dynamic array
typedef struct Dynarray {
	size_t size;
	int* ptr;
} Dynarray;

// Create a new dynamic array with size amount of elements
Dynarray dynarray_create(size_t size);

// Append value to the end of the dynamic array dynarray
void dynarray_append(Dynarray* dynarray, int value);

// Prepend value to the end of the dynamic array dynarray
void dynarray_prepend(Dynarray* dynarray, int value);

// Delete the item at the end of a dynamic array
void dynarray_deleteEnd(Dynarray* dynarray);

// Delete the item at the start of a dynamic array
void dynarray_deleteStart(Dynarray* dynarray);

#endif // DYNARRAY_H_
