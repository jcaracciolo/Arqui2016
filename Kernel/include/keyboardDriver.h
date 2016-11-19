//
// Created by juanfra on 11/06/16.
//

#ifndef KEYBOARDDRIVER_H
#define KEYBOARDDRIVER_H


void addToBuffer();

int readBuffer();

void readFull(char * buffer, int size);

condVar_t * getSTDINCondVar();


#endif //KEYBOARDDRIVER_H
