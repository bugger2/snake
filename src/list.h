#ifndef LIST_H
#define LIST_H

#include <stdarg.h>
#include "position.h"

typedef struct {
	int size;
	Position* data;
} List;

List new_list(int size, ...);

List add_list(List* list, Position* value);

void free_list(List* list);

#endif
