#include "mems.h"
#include <stdio.h>

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

const char * Mems_ReadFileAsString(Mems *mems, const char *filename){
	FILE *file = fopen(filename, "r");
	char *str;
	size_t size;

	if(file == NULL)
		return NULL;

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);

	str = (char *) Mems_Alloc(mems, size + 1);

	fread(str, 1, size, file);

	str[size] = '\0';

	return (const char *) str;
}
