//
// Created by julian on 11/16/16.
//

#include "include/math.h"

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
