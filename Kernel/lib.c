#include <stdint.h>
#include "./include/videoDriver.h"

#define MAX_BLOCK_PAGES 512
#define PAGE_SIZE 4*1024

static char * pointer = (char*)0x10000000;
static char * memory[MAX_BLOCK_PAGES];

typedef struct {
	void * address;
	int cantPages;
} pageBlock;

static pageBlock pageBlockTable[MAX_BLOCK_PAGES];
static int tableSize = 0;

void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

void markPages(int init, int cant) {
	int i;	
	for (i=init; i < cant+init; i++) {
		memory[i] = 1;
	}
}

void unmarkPages(int init, int cant) {
	int i;	
	for (i=init; i < cant+init; i++) {
		memory[i] = 0;
	}
}


pageBlock * findPageBlock(void * address) {
	for (int j = 0; j < MAX_BLOCK_PAGES; j++) {
		if(pageBlockTable[j].address==address){
			return &(pageBlockTable[j]);
		}
	}
	return 0;
}

void addPageBlock(void * address, int cantPages){
	if(tableSize >= MAX_BLOCK_PAGES) return;

	pageBlockTable[tableSize].address = address;
	pageBlockTable[tableSize].cantPages = cantPages;
	tableSize++;
}

void deletePageBlock(void * address){
	for (int j = 0; j < MAX_BLOCK_PAGES; j++) {
		if(pageBlockTable[j].address==address){
			pageBlockTable[j] = pageBlockTable[tableSize-1];
			tableSize--;
			print("\npage deleted from table");
			return;
		}
	}
}


void * allocate(int bytes){
	char * temp = pointer;
	pointer += bytes;
	return temp;
}

int pageIndexToMemory(int index) {
	return pointer + index * PAGE_SIZE;
}

int memoryToPageIndex(void * address) {
	for (int j = 0; j < MAX_BLOCK_PAGES; j++) {
		if(pageBlockTable[j].address==address){
			return j;
		}
	}
	return -1;
}

void copyBlocks(int destIndex, int originIndex, int cantPages) {
	memcpy(pageIndexToMemory(destIndex), pageIndexToMemory(originIndex), cantPages*PAGE_SIZE);
}

/* returns index of memory array where there are cantPages free consecutive blocks */
int getFreeBlock(int cantPages) {
	int i = 0, memInit = -1;
	int notFound = 1;
	int counter = 0;

	for(i = 0; notFound ; i++) {
		if (memory[i] == 0) {
			if (counter == 0) {
				memInit = i;
			}
			counter++;
			if (counter == cantPages) {
				return memInit;
			}
		} else {
			counter = 0;
		}
	}
	return -1;
}

void * allocatePages(int * address, int cantPages){

	pageBlock * block = findPageBlock(address);

	if(block == (pageBlock *)0) {
		// address was not allocated
		int memInit = getFreeBlock(cantPages);
		markPages(memInit, cantPages);
		addPageBlock(address, cantPages);
		print("\npointer:"); printNum(memInit);
		return pageIndexToMemory(memInit);
	}

	if(block->cantPages < cantPages) {
		int memInit = getFreeBlock(cantPages);
		markPages(memInit, cantPages);
		unmarkPages(memoryToPageIndex(address), block->cantPages);
		deletePageBlock(address);
		if(findPageBlock(address) != 0) {
			print("NO LO BORRO!");
		}
		addPageBlock(pageIndexToMemory(memInit), cantPages);
		print("\npointer(realloc):"); printNum(memInit);
		return pageIndexToMemory(memInit);
	}
	print("\nno entro a nada");
	return address;
	
}