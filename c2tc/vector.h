#ifndef vector_h
#define vector_h

/// <summary>
///  Vector struct
/// </summary>
typedef struct
{
	void** items;
	int32 capacity;
	int32 total;
} vector;

/// <summary>
///  This function inits a vector, use after declaring one
/// </summary>
void vector_init(vector* v);
/// <summary>
///  This function returns the number of items in a vector
/// </summary>
int32 vector_total(vector* v);
/// <summary>
///  This function is for resizing a vector, used internally, no need to use manually
/// </summary>
void vector_resize(vector* v, int32 capacity);
/// <summary>
///  This function adds an item at the end of the vector
/// </summary>
void vector_add(vector* v, void* item);
/// <summary>
///  This function places an item at index position in the vector
/// </summary>
void vector_set(vector* v, int32 index, void* item);
/// <summary>
///  This function returns an item at index position in the vector
/// </summary>
void* vector_get(vector* v, int32 index);
/// <summary>
///  This function deletes an item at index position
/// </summary>
void vector_delete(vector* v, int32 index);
/// <summary>
///  Use this function to free a vector
/// </summary>
void vector_free(vector* v);

/// <summary>
///  This function tests vector
/// </summary>
void testvector();
#endif