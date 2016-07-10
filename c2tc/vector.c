#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inttypes.h"
#include "vector.h"
#include "microtest.h"
//Implementation of a vector. Based on tutorial
//by Edd Mann

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
	v->items = NULL;
}

void testvector()
{
	vector* v = malloc(sizeof(vector));
	vector_init(v);
	tiny_file();
	vector_add(v, "test");
	tiny_assert("test if vector_add() works properly", vector_total(v) == 1);
	tiny_assert("test if vector_get() works properly", (strcmp(vector_get(v, 0), "test") == 0));
	vector_set(v, 0, "test2");
	tiny_assert("test if vector_set() works properly", (strcmp(vector_get(v, 0), "test2") == 0));
	tiny_assert("test if vector_total() works properly", vector_total(v) == 1);
	vector_delete(v, 0);
	tiny_assert("test if vector_delete() works properly", vector_total(v) == 0);
	vector_free(v);
	tiny_assert("test if vector_free() is working properly", !v->items);
}
