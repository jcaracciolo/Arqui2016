/*
 * types.h
 *
 *  Created on: Apr 17, 2010
 *      Author: anizzomc
 */

#ifndef TYPES_H_
#define TYPES_H_


typedef unsigned char byte;
typedef short int word;
typedef int dword;

typedef struct {
	int x;
	int y;
} coord_t;



#pragma pack(push)
#pragma pack (1) 		/* Alinear las siguiente estructuras a 1 byte */

/* Descriptor de segmento */
typedef struct {
  word limit,
       base_l;
  byte base_m,
       access,
       attribs,
       base_h;
} DESCR_SEG;


/* Descriptor de interrupcion */
typedef struct {
  word      offset_l,
            selector;
  byte      cero,
            access;
  word	    offset_h;
} DESCR_INT;

/* IDTR  */
typedef struct {
  word  limit;
  dword base;
} IDTR;

#pragma pack(pop)

#endif /* TYPES_H_ */
