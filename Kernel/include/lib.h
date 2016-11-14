#ifndef LIB_H
#define LIB_H

#define PAGE_SIZE 4*1024

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
void * malloc(int bytes);
void * free(void * p);
void * allocate(int bytes);

char *cpuVendor(char *result);

void * allocatePages(int cantPages);
void * reallocatePages(int * address, int cantPages);

#define MEMBEGIN 0x10000000

#endif