//minimalistic object look-alike library
//TODO move llist to util.c/h
#ifndef ooc_h
#define ooc_h

#include "util.h"

//macros
#define new(type) (type*)_new(#type, sizeof(type))
#define classdef typedef struct { char* _type; uint64_t _hash; struct
#define class(type) ;} type;

#define box_value(value) _box(sizeof(value), value)
#define unbox_value(b) b->content


//types
typedef struct
{
	char* _type;
	int64_t _hash;
} object;

typedef struct _llist
{
	void* data;
	struct _llist* n;
} llist;

//box
classdef
{
	uint64_t size;
	void* content;
} class(box);

//global variables
llist* hashes;

//linked list crap
void llist_put(llist* l, void* data);
void* llist_get(llist* l, int32_t index, int32_t carry);
llist* llist_new(void* first);
int32_t llist_total(llist* l, int32_t carry);

//utils
uint8_t rand256();
uint64_t rand64();
uint64_t get_hash();

//main functions
object* _new(char* type, int64_t size);
void delete(object* obj);
box* _box(uint64_t size, void* content);

void init_ooc();

#endif
