#include <stdlib.h>
#include <stdio.h>
#include "list.h"

List add_list(List* list, Position* value) {
	list->size++;
	list->data = (Position*)realloc(list->data, list->size);
	list->data[list->size-1] = *value;

	return *list;
}

List new_list(int size, ...) {
	List ret = {
		.size = size,
		.data = (Position*)calloc(size, sizeof(Position)),
	};
	
	if(ret.data == NULL && size > 0) {
		fprintf(stderr, "Could not assign data for list");
		exit(1);
	}

	va_list ptr;
	va_start(ptr, size);

	for(int i = 0; i < size; i++) {
		ret.data[i] = va_arg(ptr, Position);
	}

	va_end(ptr);

	return ret;
}

void free_list(List* list) {
	list->size = 0;
	free(list->data);
}
