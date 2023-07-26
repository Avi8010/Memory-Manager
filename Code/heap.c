

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include "heap.h"
#include "chunk.h"
#include <unistd.h>
#include <string.h>


#define MIN_UNITS 1024

#define ARRAY_SIZE 512

#define MIN_UNITS_IN_CHUNK 1024



static chunkPtr memStart = NULL;

static chunkPtr memEnd = NULL;

static chunkPtr memArray[ARRAY_SIZE];


//insert the chunk of memory at the right place in the array of linked list
void insertInArray(chunkPtr ch){                    
        chunkPtr arrPtr, prevChunk, q = NULL;
        
		//get size of chunk to be inserted in memory.
        size_t chSize = getChunkUnits(ch);
        size_t temp = chSize;


		//Determine which part of array chunk should be inserted
		//if chunk size is greater than array size set the chunk at the end of array
        if(chSize > ARRAY_SIZE - 1){
            arrPtr = memArray[ARRAY_SIZE - 1];
            temp = ARRAY_SIZE - 1;
        }
        else
            arrPtr = memArray[chSize];


		//traversing the selected part of array
		//till the end of array or getting chunksize greater than equal to the chunk to be inserted. 
        while(arrPtr != NULL && getChunkUnits(arrPtr) < chSize){
            
            q = arrPtr;//q keeps track of prev chunk.
            arrPtr = getNextInList(ch);
        }

		//inserting the chunk between the array
        setNextInList(ch, arrPtr);
        if(arrPtr){
            prevChunk = getPrevInList(arrPtr);
            setPrevInList(ch, prevChunk);
            if(!prevChunk)
                memArray[temp] = ch;

            setPrevInList(arrPtr,ch);
            if(q)
                setNextInList(q,ch);
        }
        else{
        	
            if(memArray[temp] == NULL){
            	
                memArray[temp] = ch;
                setPrevInList(ch,NULL);
            }

            else{
                setNextInList(q,ch);
                setPrevInList(ch,q);
            }
        }

        return;
}


//This function is used to unite or coalesce the contiguous memory. It prevents fragmentation of free memory.

chunkPtr coalesce(chunkPtr p, chunkPtr q){
	//if p and q chunks are not free
	if(!p)
		return q;
	else if(!q)
		return p;
	//if both chunks are free merge them to single chunk
	size_t pSize = getChunkUnits(p);
	size_t qSize = getChunkUnits(q);

	size_t total = pSize + qSize;

	setChunkUnits(p ,total);
	return p;
}


int getMemIndex(size_t s){

	if(s >= ARRAY_SIZE)
		return ARRAY_SIZE-1;
	else
		return (int)(s);

}


//When the memory is given to use for user , it is removed from the free list

void removeFromList(chunkPtr p){
	//finding prev and next chunks of current chunk
	chunkPtr prevChunk = getPrevInList(p);
	chunkPtr nextChunk = getNextInList(p);
	
	if(prevChunk)
		setNextInList(prevChunk,nextChunk);
	else
	    //if chunk to be remove is very fist in memory.
		memArray[getMemIndex(getChunkUnits(p))] = nextChunk;

	if(nextChunk)
		setPrevInList(nextChunk,prevChunk);
		
	return;
			
}


void printHeap(){

	/*int i;
	chunkPtr ch;

	for(i = 0; i < ARRAY_SIZE; i++){
		ch = memArray[i];
		while(ch){
			printf("i = %d, units = %d\n", i, (int)getChunkUnits(ch));
			ch = getNextInList(ch);
		}
	}
	printf("\n");
	return;*/
	
	chunkPtr curr = memStart;
    int totalChunks = 0;
    int usedChunks = 0;
    int freeChunks = 0;
    int totalUnits = 0;
    int unusedChunks =0;
    int cnt=1;

    printf("Chunk Status:\n");

    while (curr != NULL) {
        totalChunks++;
        if(cnt>512){
         break;
        }
        if (chunkStatus(curr)) {
            usedChunks++;
            printf("\x1b[31m■\x1b[0m ");  // Red square box representing an allocated chunk
        } else if (curr->size > sizeof(chunk)) {
            unusedChunks++;
            printf("\x1b[32m■\x1b[0m ");
             // Yellow square box representing a freed chunk
        } else {
            freeChunks++;
            printf("\x1b[33m■\x1b[0m "); 
              // Green square box representing a free chunk
        }
        if(cnt%64==0){
          printf("\n");
        }
        cnt++;
        
        curr = getNextMem(curr, memEnd);
       
    }

    printf("\n");
    /*printf("Total Chunks: \x1b[37m%d\x1b[0m\n", totalChunks);
    printf("Used Chunks:  \x1b[31m%d\x1b[0m\n", usedChunks);
    printf("Unused Chunks: \x1b[32m%d\x1b[0m\n", unusedChunks);
    printf("Free Chunks: \x1b[33m%d\x1b[0m\n", freeChunks);
    printf("\n");*/
    printf("\n");
    printf("Chunk Statistics:\n");
    printf("+--------------+-------------+---------------+--------------+\n");
    printf("| \x1b[31mInuse Chunks\x1b[0m | \x1b[33mFree Chunks\x1b[0m | \x1b[32mUnused Chunks\x1b[0m | Total chunks |\n");
    printf("+--------------+-------------+---------------+--------------+\n");
    printf("|     \x1b[31m%d\x1b[0m        |     \x1b[33m%d\x1b[0m       |      \x1b[32m%d\x1b[0m        |      \x1b[37m%d\x1b[0m       |\n", usedChunks, freeChunks,unusedChunks, totalChunks);
    printf("+--------------+-------------+---------------+--------------+\n\n");
    //printf("%d\n",totalUnits);
    printf("\n");
    
}



//This function is called to give a system call and request for more memory from the operating syatem.
chunkPtr moreMemory(size_t reqUnits){   
	//request for memory from operating syatem
    chunkPtr chunk;
    chunkPtr prevMem;
    chunkPtr newEnd;
    	
    if(reqUnits < MIN_UNITS)
        reqUnits = MIN_UNITS;

    newEnd = (chunkPtr)((char*)memEnd + reqUnits*chunkSize());

    if(newEnd < memEnd)
        return NULL;
	//if program breaks
    if(brk(newEnd) == -1)
        return NULL;

    chunk = memEnd;
    memEnd = newEnd;
    prevMem = getPrevMem(chunk,memStart);

    chunk->size = 0;
    
    setChunkUnits(chunk, reqUnits);
    chunkSetStatus(chunk, 0);

    if( prevMem && !(chunkStatus(prevMem))){
	    removeFromList(prevMem);
	    chunk = coalesce(prevMem,chunk);
    }

    insertInArray(chunk);

    assert(chunkStatus(chunk) == 0);

    return chunk;
}


//This function is called when malloc finds a chunk which is greater than or equal to required size
//modifies the chunk as required for the user and returns it

chunkPtr getChunk(chunkPtr p, size_t reqUnits){

	size_t pSize = getChunkUnits(p);

	size_t extra = pSize - reqUnits;
	
	chunkPtr extraChunk, tempChunk = p;
	removeFromList(p);

	//if the extra memory is less than required memory,return with no creation of chunk
	if(pSize < reqUnits + MIN_UNITS_IN_CHUNK){

		chunkSetStatus(tempChunk,1);

		return tempChunk;
	}
	

	setChunkUnits(tempChunk, reqUnits);
	chunkSetStatus(tempChunk, 1);

	extraChunk = getNextMem(tempChunk, memEnd);//extracting extra memory chunk

	setChunkUnits(extraChunk, extra);// set extra memory in array
	chunkSetStatus(extraChunk, 0);//set memory is free

	insertInArray(extraChunk);//inserting in array of chunks.
    
	return tempChunk; //pointer to the current required memory chunk

}


//malloc2 function : allocates required bytes to the user 
void* malloc2(size_t dsize){
	
	size_t uSize = chunkSize();
	size_t units ;
	
	int index;
	chunkPtr tempChunk;

	if(dsize == 0)
		return NULL;
	units = (dsize - 1)/uSize + 1;
	units += 2;

	//initialising the start and end of whole heap memory
	
	//if function calls first time.memory needs initialisation.
	if(!memStart)
		memEnd = memStart = (chunkPtr)sbrk(0);
	
	//loop for searching free list of memory
	for(index = getMemIndex(units); index < ARRAY_SIZE; index++){
		tempChunk = memArray[index];
		//printf(" %d ", index);

		while(tempChunk){
			//if get free chunk as needed
			if(getChunkUnits(tempChunk) >= index){
				//getChunk splits memory in two one for allocation and other for added back to free list
				tempChunk = getChunk(tempChunk, units);

				return (void *)(tempChunk + 1);
			}

			tempChunk = getNextInList(tempChunk); //traversing the list through next chunk
		}
	}

	//additonal memory if no such chunk is found
	tempChunk = moreMemory(3);

	//if fail to allocate more meomory
	if(tempChunk == NULL){
		return NULL;

	}
	
	tempChunk = getChunk(tempChunk, units);
	//printf("\n%p no\n", memArray[15]);

	return (void *)(tempChunk + 1);

}


// free2 function : frees the memory , changes status to free and attach the freed memory in linked list
// of free memory for further use

void free2(void *mem){
	// mem pointer which is previously allocated
	//if me memory is null
	if(!mem)
		return;

	chunkPtr tempChunk, prev, next;

	tempChunk = (chunkPtr)((char*)mem - chunkSize());

	chunkSetStatus(tempChunk, 0);//sets the status of current chunk as free.

	prev = getPrevMem(tempChunk, memStart); //get prev chunk of current chunk

	//if prev and current chunk both are free then merge them to one single chunk to reduce the fragmentation
	if(prev && !(chunkStatus(prev))){
		removeFromList(prev);
		tempChunk = coalesce(prev, tempChunk);
	}
	
	next = getNextMem(tempChunk, memEnd);

	//if next and current chunk both are free then merge them to one single chunk to reduce the fragmentation
	if(next && !(chunkStatus(next))){
		removeFromList(next);
		tempChunk = coalesce(tempChunk, next);
	}

	//insert coalesc chunk in the list
	insertInArray(tempChunk);

	return;

}


// Calloc2 function : call malloc and initialise each byte to 0
void* calloc2(size_t count, size_t siz){
	//count = number of elements to be allocate
	//size = size of each element
	char* memory;
	size_t totalSiz = count*siz;
	
	memory = (char*)malloc2(totalSiz);//allocates memory of reqquired size
	
	//if allocation successfull initialises memory block to 0 using memset
	if(memory){
		memory = memset(memory, 0, totalSiz);
	}

	return (void*)memory;//return pointer to the allocated and the initialized memory block
}
	



//Realloc2 function : Reallocates memory , spread or compress according to user needs
//If user wants to compress no changes in memory pointer , but if user wants to increase the memory size 
//changing the memory pointer and alloting new bytes , copying the prev data to newly alloted memory
			
			
void* realloc2(void *ptr, size_t reqSize){
	//ptr = ponter to prev allocated memory, reqSize=size to be reallocate
	chunkPtr curr, new, temp;
	size_t currUnits, reqUnits, extraUnits;

	curr = (chunkPtr)ptr;
	curr = curr - 1;
	
	currUnits = getChunkUnits(curr);//current aloocated units
	
	reqUnits = (reqSize - 1)/chunkSize() + 3;

	if(reqUnits < currUnits - 3){			//if user wants reallocated memory to be less than previous
		
		setChunkUnits(curr, reqUnits);
		
		temp = getNextMem(curr, memEnd);
		extraUnits = currUnits - reqUnits;

		setChunkUnits(temp, extraUnits);
		chunkSetStatus(temp, 1);

		free2(temp + 1);

		return curr;

	}

	if(reqUnits <= currUnits)
		return curr;

	new = (chunkPtr)malloc2(reqSize);         //calling malloc2 to allocate larger memory than previous

	if(new){ // copies contents from old memory block to the new memory block 
		new = (chunkPtr)strncpy((char*)new, (char*)curr, (currUnits-2) * chunkSize());

		free2(curr + 1);//frres the old memory block

	}
	
	return (void*)new;//poniter to new memory block

}

		

