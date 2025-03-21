#ifndef MEMS_H
#define MEMS_H

#include <stddef.h>

/* Stack de alocação de memória.
 * Evitar usar malloc e fragmentar a memória. */

typedef struct {
	char *block;
	size_t size;
	size_t top;
} Mems;

Mems Mems_Create(void *block, size_t size);

void * Mems_Alloc(Mems *mems, size_t size);

void Mems_Free(Mems *mems);

void * Mems_GetTop(Mems *mems);

size_t Mems_SaveState(Mems *mems);

void Mems_RestoreState(Mems *mems, size_t old_state);

const char * Mems_ReadFileAsString(Mems *mems, const char *filename);

#endif
