#ifndef CHUNK_H
#define CHUNK_H

#include <stddef.h>
#include <unistd.h>


//defining chunk structure
typedef struct chunk{
    size_t size; //size of the chunk in bytes
    struct chunk* neighbr;  //pointer to the next chunk in the heap
}chunk;

typedef chunk* chunkPtr;  //chunkPtr

size_t chunkSize();  //get chunksize
int chunkStatus(chunkPtr c);  //get chunk status 0 or 1 . 1->chunk is allocated, 0->chunk is free
void chunkSetStatus(chunkPtr ch,int stat);  //sets chunk sttatus by stat value
size_t getChunkUnits(chunkPtr ch);  //get chunk units where each unit is of 2 bytes.
void setChunkUnits(chunkPtr ch,size_t units);  //set size of chunk in units.
chunkPtr getNextInList(chunkPtr ch);  //get next chunk pointer
chunkPtr getPrevInList(chunkPtr ch);    //get prev chunk pointer
void setNextInList(chunkPtr ch, chunkPtr next);    //set next neighbr as next
void setPrevInList(chunkPtr ch, chunkPtr prev);    // set prev neigbhr as prev
chunkPtr getPrevMem(chunkPtr ch, chunkPtr start);   //get prev chunk
chunkPtr getNextMem(chunkPtr ch, chunkPtr End);    //et next chunk

#endif // CHUNK_H




