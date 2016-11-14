//
// Created by juanfra on 13/11/16.
//

#ifndef ARQUI2016_SYNC_H
#define ARQUI2016_SYNC_H

int createMutex(char* name);

int releaseMutex(int mutex);

int waitMutex(int mutex);

#endif //ARQUI2016_SYNC_H
