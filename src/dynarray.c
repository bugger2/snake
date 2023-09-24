#include "dynarray.h"
#include <stdlib.h>
#include <stdio.h>

Dynarray dynarray_create(size_t size) {
	return (Dynarray) {
		.size = size,
		.ptr = malloc(size * sizeof(int)),
	};
}

void dynarray_append(Dynarray* dynarray, int value) {
	dynarray->size++;
	if(dynarray->ptr != NULL)
		dynarray->ptr = realloc(dynarray->ptr, dynarray->size * sizeof(int));
	else
		dynarray->ptr = malloc(dynarray->size * sizeof(int));
	dynarray->ptr[dynarray->size-1] = value;
}

void dynarray_prepend (Dynarray* dynarray, int value) {
	dynarray->size++;
	if(dynarray->ptr != NULL)
		dynarray->ptr = realloc(dynarray->ptr, dynarray->size * sizeof(int));
	else
		dynarray->ptr = malloc(dynarray->size * sizeof(int));
	for (unsigned int i = dynarray->size - 1; i > 0; i--)
		dynarray->ptr[i] = dynarray->ptr[i-1];
	dynarray->ptr[0] = value;
}

void dynarray_deleteEnd (Dynarray* dynarray) {
	dynarray->size -= dynarray->size == 0 ? 0 : 1;
	if(dynarray->ptr != NULL)
		dynarray->ptr = realloc(dynarray->ptr, dynarray->size * sizeof(int));
}

void dynarray_deleteStart (Dynarray* dynarray) {
	dynarray->size -= dynarray->size == 0 ? 0 : 1;
	for(unsigned int i = 0; i < dynarray->size; i++)
		dynarray->ptr[i] = dynarray->ptr[i+1];
	if(dynarray->ptr != NULL)
		dynarray->ptr = realloc(dynarray->ptr, dynarray->size * sizeof(int));
}
