#ifndef vector_h
#define vector_h

typedef struct
{
	void** items;
	int32 capacity;
	int32 total;
} vector;

void vector_init(vector* v);
int32 vector_total(vector* v);
void vector_resize(vector* v, int32 capacity);
void vector_add(vector* v, void* item);
void vector_set(vector* v, int32 index, void* item);
void* vector_get(vector* v, int32 index);
void vector_delete(vector* v, int32 index);
void vector_free(vector* v);

#endif