#include "mems.h"

Mems Mems_Create(void *block, size_t size){
	Mems mems = {block, size, 0};

	return mems;
}

void * Mems_Alloc(Mems *mems, size_t size){
	char *location;

	if(mems->top >= mems->size)
		return NULL;

	location = mems->block + mems->top;
	mems->top += size;

	return (void *) location;
}

void Mems_Free(Mems *mems){
	mems->top = 0;
}
