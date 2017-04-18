//minimalistic object look-alike library
#include <stdio.h>
#include <stdlib.h>

#include "ooc.h"

llist* llist_new(void* first)
{
	if (first == NULL) return NULL;
	llist* l = malloc(sizeof(llist));
	l->data = first;
	l->n = NULL;
	return l;
}
void llist_put(llist* l, void* data)
{
	if (!l) return;
	if (l && l->n == NULL)
	{
		l->n = malloc(sizeof(llist));
		l->n->data = data;
		l->n->n = NULL;
	}
	else llist_put(l->n, data);
}
int32_t llist_total(llist* l, int32_t carry)
{
	if (!l) return 0;
	if (l->n != NULL) return llist_total(l->n, carry + 1);
	else return carry + 1;
}
void* llist_get(llist* l, int32_t index, int32_t carry)
{
	if (!l) return NULL;
	if (carry > index) return NULL;
	if (carry < index && l->n) return llist_get(l->n, index, carry + 1);
	if (carry == index && l->data != NULL) return l->data;
	return NULL;
}

//utils
uint8_t rand256()
{
	static unsigned const limit = RAND_MAX - RAND_MAX % 256;
	uint8_t result = rand();
	while (result >= limit)
		result = rand();
	return result % 256;
}
uint64_t rand64()
{
	uint64_t results = 0ULL;
	for (int count = 8; count > 0; --count)
		results = 256U * results + rand256();
	return results;
}
int8_t contains(llist* l, uint64_t n)
{
	for (uint64_t i = 0; i < llist_total(l, 0); i++)
		if ((uint64_t)llist_get(l, i, 0) == n) return 1;
	return 0;
}

uint64_t get_hash()
{
	uint64_t hash;
	while (contains(hashes, (hash = rand64()))) {}
	llist_put(hashes, (void*)hash);
	return hash;
}

void init_ooc()
{
	if (!(hashes = llist_new(0)))
	{
		puts("ooc failed to init");
		exit(1);
	}
}

object* _new(char* t, int64_t s)
{
	object* obj = malloc(s);
	obj->_type = t;
	obj->_hash = get_hash();
	return obj;
}

void delete(object* obj)
{
	free(obj->_type);
	free(obj);
}

box* _box(uint64_t s, void* c)
{
	box* b = new(box);
	b->size = s;
	b->content = c;
	return b;
}
