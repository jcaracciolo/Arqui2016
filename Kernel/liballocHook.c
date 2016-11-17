//
// Created by juanfra on 16/11/16.
//

#include "include/mutex.h"
#include "include/liballoc.h"
#include "include/buddyMemManager.h"
#include "include/videoDriver.h"
#include <stddef.h>


static int mutex;

void initializeMalloc(){

    mutex=getMutex("__MALLOC__MUTEX");
}
int liballoc_lock()
{
    lockMutex(mutex);
    return 0;
}

int liballoc_unlock()
{
    unlockMutex(mutex);
    return 0;
}

void* liballoc_alloc( size_t pages )
{
    return allocatePages(pages);
}

int liballoc_free( void* ptr, size_t pages )
{
    return buddyFree(ptr);
}


