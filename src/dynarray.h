#ifndef DYNARRAY_H_
#define DYNARRAY_H_

#include <stdarg.h>
#include <stdlib.h>

typedef struct Dynarray {
	size_t size;
	int* ptr;
	void (*append)(struct Dynarray* ptr, int value);
	void (*prepend)(struct Dynarray* ptr, int value);
	void (*deleteEnd)(struct Dynarray* ptr);
	void (*deleteStart)(struct Dynarray* ptr);
} Dynarray;

Dynarray dynarray_create(size_t size, ...); // pretty please do not initialize a dynamic array with more arguments than size

#endif // DYNARRAY_H_
