
#include "chunk.h"
#include <stddef.h>


//returns th size of chunk
size_t chunkSize(){

    return sizeof(chunk);

}


//returns the flag according to status of chunk - free or allocated.
int chunkStatus(chunkPtr c){
    //lsb of size & 1U gives the flag 
    return c->size & 1U;

}


//sets the chunk status of ch with the status value stat.
void chunkSetStatus(chunkPtr ch,int stat){
    //and operatio clears the lsb of size
    ch->size &= ~1U;

    //or operation sets the lsb with stat value
    ch->size |= stat;
    return;
}

//returns the total number of chunkunits, where each chunk is of 2 bytes.
size_t getChunkUnits(chunkPtr ch){
    //size = total size of chunk ch
    return (ch->size)>>1;
}

void setChunkUnits(chunkPtr ch,size_t units){
    //clears or free the inuse chunk
    ch->size &= 1U;

    //set size of chunk in units from bytes
    ch->size |= units<<1U;

    //sets the last unit of chunk size equal to units, marking end of chunk.
    (ch+units-1)->size = units;
    return;
}

//returns the pointer to the next chunk in the memory
chunkPtr getNextInList(chunkPtr ch){

    return ch->neighbr;

}

//returns the previous chunk pointer int the memory
chunkPtr getPrevInList(chunkPtr ch){
    //calls getChunkUnits() function which returns the size of current chunk in units.
    //add this size to the input chunk to get the end pointer of current chunk.
    return (ch + getChunkUnits(ch) - 1)->neighbr;

}


//sets the next neighbr of the chunk ch as net
void setNextInList(chunkPtr ch, chunkPtr next){
    
    ch->neighbr = next;
    return ;
}

//sets the  prev neighbr of chunk ch as prev
void setPrevInList(chunkPtr ch, chunkPtr prev){

    (ch + getChunkUnits(ch) - 1)->neighbr = prev;
    return;
}

//return prev chunk
chunkPtr getPrevMem(chunkPtr ch, chunkPtr start){
    //variabl to hold address of prev chunk in memory
    chunkPtr prevTemp;

    size_t temp = chunkSize(ch);

    //if ch is first chunk in the memory
    if(ch == start)
        return NULL;

    //calculates address of prev chunk in memory
    prevTemp = (chunkPtr)((char*)ch - temp);

    prevTemp = ch - prevTemp->size;
    return prevTemp;

}

//returns next chunk
chunkPtr getNextMem(chunkPtr ch, chunkPtr End){
	
	chunkPtr nextTemp;

	size_t temp = getChunkUnits(ch);
	
	nextTemp = ch + temp;
	if(nextTemp == End)
		return NULL;

	return nextTemp;
}
	

