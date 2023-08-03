#include "dynarray.h"
#include <stdlib.h>
#include <stdio.h>

void dynarray_append(Dynarray* dynarray, int value) {
	dynarray->size++;
	if(dynarray->ptr != NULL)
		dynarray->ptr = realloc(dynarray->ptr, dynarray->size * sizeof(int));
	else
		dynarray->ptr = malloc(dynarray->size * sizeof(int));
	dynarray->ptr[dynarray->size] = value;
}

void dynarray_prepend (Dynarray* dynarray, int value) {
	dynarray->size++;
	printf("The dynarray->ptr = %p", dynarray->ptr);
	if(dynarray->ptr != NULL)
		dynarray->ptr = realloc(dynarray->ptr, dynarray->size * sizeof(int));
	else
		dynarray->ptr = malloc(dynarray->size * sizeof(int));
	for (int i = dynarray->size - 1; i > 0; i--)
		dynarray->ptr[i] = dynarray->ptr[i-1];
	dynarray->ptr[0] = value;
}

void dynarray_deleteEnd (Dynarray* dynarray) {
	dynarray->size == 0 ? dynarray->size = 0 : dynarray->size--;
	if(dynarray->ptr != NULL)
		dynarray->ptr = realloc(dynarray->ptr, dynarray->size * sizeof(int));
}

void dynarray_deleteStart (Dynarray* dynarray) {
	if(dynarray->size > 0) {
		for(unsigned int i = 0; i < dynarray->size; i++)
			dynarray->ptr[i] = dynarray->ptr[i+1];
		if(dynarray->ptr != NULL) dynarray->ptr = realloc(dynarray->ptr, dynarray->size * sizeof(int));
	}
}

Dynarray dynarray_create(size_t size, ...) {
	va_list args;
	va_start(args, size);

	Dynarray ret = {
		ret.size = size,
		ret.ptr = malloc(size * sizeof(int)),
		ret.append = dynarray_append,
		ret.prepend = dynarray_prepend,
		ret.deleteEnd = dynarray_deleteEnd,
		ret.deleteStart = dynarray_deleteStart,
	};

	for(size_t i = 0; i < size; i++) {
		ret.ptr[i] = va_arg(args, int);
	}

	va_end(args);

	return ret;
}
