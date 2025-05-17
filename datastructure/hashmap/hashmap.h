#ifndef HASHMAP
#define HASHMAP

#include <stdio.h>
#include <stdlib.h>


void* createHashMap(void* datatype, int size)
{
	void* hashmap = malloc(sizeof(datatype) * size);
	return hashmap;
}


#endif // HASHMAP
