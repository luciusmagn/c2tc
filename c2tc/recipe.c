#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "inttypes.h"
#include "recipe.h"
#include "mpc.h"
#include "vector.h"

void processrecipe()
{
	puts("test");
	for (int32 i = 0; i < recipe->count; i++)
	{
		processtarget(vector_get(recipe->targets, i));
	}
}

void processtarget(target* trg)
{
	puts(trg->name);
}