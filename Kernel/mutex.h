//
// Created by juanfra on 13/11/16.
//

#ifndef ARQUI2016_MUTEX_H
#define ARQUI2016_MUTEX_H

int createMutex(char* name);

int releaseMutex(int mutex);

int waitMutex(int mutex);

int releaseMutex(int mutex);

#endif //ARQUI2016_MUTEX_H