//
// Created by juanfra on 13/11/16.
//

#ifndef ARQUI2016_SYNC_H
#define ARQUI2016_SYNC_H

int createMutex(char* name);

int lockMutex(int mutex);

int unlockMutex(int mutex);

int releaseMutex(int mutex);

#endif //ARQUI2016_SYNC_H
