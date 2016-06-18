#include <stdio.h>
#include <stdlib.h>
#include "inttypes.h"
#include "vector.h"
//Implementation of a vector in C2. Based on tutorial for
//C by Edd Mann

void vector_init(vector* v)
{
	v->capacity = 4;
	v->total = 0;
	v->items = malloc(sizeof(void *) * v->capacity);
}

int32 vector_total(vector* v)
{
	return v->total;
}

void vector_resize(vector* v, int32 capacity)
{
	void** items = realloc(v->items, sizeof(void *) * capacity);

	if (items != NULL)
	{
		v->items = items;
		v->capacity = capacity;
	}
}

void vector_add(vector* v, void* item)
{
	if (v->capacity == v->total)
		vector_resize(v, v->capacity * 2);
	v->items[v->total++] = item;
}

void vector_set(vector* v, int32 index, void* item)
{
	if (index >= 0 && index < v->total)
		v->items[index] = item;
}

void* vector_get(vector* v, int32 index)
{
	if (index >= 0 && index < v->total)
		return v->items[index];
	return NULL;
}

void vector_delete(vector* v, int32 index)
{
	if (index < 0 || index >= v->total)
		return;

	v->items[index] = NULL;

	for (int32 i = 0; i < v->total - 1; i++)
	{
		v->items[i] = v->items[i + 1];
		v->items[i + 1] = NULL;
	}

	v->total--;

	if (v->total > 0 && v->total == v->capacity / 4)
		vector_resize(v, v->capacity / 2);
}

void vector_free(vector* v)
{
	free(v->items);
}