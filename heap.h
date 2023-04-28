#ifndef HEAP_MANAGER
#define HEAP_MANAGER

#include <stddef.h>
#include <stdio.h>

void* malloc2(size_t size);  //calls malloc2 function to allocate the memory.
void printHeap();   //print the current heap status.
void free2(void* mem);   // free the allocated memory.
void* calloc2(size_t count, size_t size);    //calloc2 continuos allocation of memory.
void* realloc2(void *ptr, size_t reqSize);    //reallocate the memory as per request.

#endif //HEAP_MANAGER
