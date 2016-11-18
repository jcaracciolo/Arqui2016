//
// Created by julian on 11/16/16.
//

#include "include/math.h"

static unsigned long int next = 1;

void cos(float x, float * res){
    if( x < 0.0f )
        x = -x;
    while( M_PI < x )
        x -= M_2_PI;
    *res = 1.0f - (x*x/2.0f)*( 1.0f - (x*x/12.0f) * ( 1.0f - (x*x/30.0f) * (1.0f - x*x/56.0f )));
    return;
}

void sin(float x, float * res){
    cos(x-M_PI_2, res);
    return ;
}

//http://stackoverflow.com/questions/4768180/rand-implementation

int randM(void) // RAND_MAX assumed to be 32767
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next/65536) % 32768;
}

void srand(unsigned int seed)
{
    next = seed;
}

int randBound(int min, int max){
    return randM()%(max-min)+min;
}