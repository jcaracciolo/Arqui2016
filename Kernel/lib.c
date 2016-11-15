#include <stdint.h>
#include "./include/videoDriver.h"
#include "lib.h"

#define MAX_BLOCK_PAGES 512

static char * pointer = (char*)MEMBEGIN;
static char * mallocPointer = (char*)MEMBEGIN + MAXMEMORY;
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
	for (int j = 0; j < tableSize; j++) {
		if(pageBlockTable[j].address == address) {
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

void * malloc(int bytes) {
	allocate(bytes);
}

void * free(void * p) {
	
}

void * allocate(int bytes){
	char * temp = mallocPointer;
	mallocPointer += bytes;
	return temp;
}

int pageIndexToMemory(int index) {
	return pointer + index * PAGE_SIZE;
}

int memoryToPageIndex(void * address) {
	int pageIdx = 0;
	for (int j = pointer; pageIdx < MAX_BLOCK_PAGES ; j+=PAGE_SIZE, pageIdx++) {
		if(j==address){
			return pageIdx;
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

/* Debug function */
void printTable() {
	print("\nTABLE:\n");
	for (int i = 0; i < tableSize; i++) {
		print("    ");
		printNum(pageBlockTable[i].address);
		print(" -> ");
		printNum(pageBlockTable[i].cantPages);
		print("\n");
	}
	print("END TABLE");
}

/* Debug function 
 * Print n elements of array memory
 */
void printPagesArray(int n) {
	print("\n");
	for (int i = 0; i < n; i++) {
		printNum(memory[i]);
	}
}



void * allocatePages(int cantPages){
	return reallocatePages(-1, cantPages);
}

/* Allocate cantPages pages. If address exists it will reallocate memory, else it will
 * allocate the first cantPages consecutive pages available.
 * @returns: position of the beggining of the block allocated
 */
void * reallocatePages(int * address, int cantPages){
	pageBlock * block = findPageBlock(address);
	if(block == (pageBlock *)0) {
		// address was not allocated
		int memInit = getFreeBlock(cantPages);
	//print("\nmeminit: "); printNum(memInit);
		markPages(memInit, cantPages);
		addPageBlock(pageIndexToMemory(memInit), cantPages);
	//print("\npointer:"); printNum(memInit);
	//printTable();
	//printPagesArray(70);
		return pageIndexToMemory(memInit);
	}

	if(block->cantPages < cantPages) {
		// reallocing more memory
		unmarkPages(memoryToPageIndex(address), block->cantPages);
		int memInit = getFreeBlock(cantPages);
	//print("\npointer(realloc):"); printNum(memInit);
		markPages(memInit, cantPages);
		deletePageBlock(address);
		addPageBlock(pageIndexToMemory(memInit), cantPages);
		copyBlocks(memInit, address, block->cantPages);
	//printTable();
	//printPagesArray(70);
		return pageIndexToMemory(memInit);
	}

	//realloc less memory
print("\nno entro a nada");
	unmarkPages(memoryToPageIndex(address) + cantPages, block->cantPages - cantPages);
//print("\n start to delete: "); printNum(memoryToPageIndex(address) + cantPages);
//print("\n delete length: "); printNum(block->cantPages); print("-"); printNum(cantPages);
	deletePageBlock(address);
	addPageBlock(address, cantPages);
//printTable();
//printPagesArray(70);
	return address;	
}