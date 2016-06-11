/***************************************************
  Defs.h
	
****************************************************/

#ifndef _defs_
#define _defs_


/* Flags para derechos de acceso de los segmentos */
#define ACS_PRESENT     0x80            /* segmento presente en memoria */
#define ACS_CSEG        0x18            /* segmento de codigo */
#define ACS_DSEG        0x10            /* segmento de datos */
#define ACS_READ        0x02            /* segmento de lectura */
#define ACS_WRITE       0x02            /* segmento de escritura */
#define ACS_IDT         ACS_DSEG
#define ACS_INT_386 	0x0E		/* Interrupt GATE 32 bits */
#define ACS_INT         ( ACS_PRESENT | ACS_INT_386 )
#define ACS_SOFT 	( ACS_PRESENT | ACS_INT_386 )

#define ACS_CODE        (ACS_PRESENT | ACS_CSEG | ACS_READ)
#define ACS_DATA        (ACS_PRESENT | ACS_DSEG | ACS_WRITE)
#define ACS_STACK       (ACS_PRESENT | ACS_DSEG | ACS_WRITE)


#endif

//
//%macro pushState 0  ; fuente: RowDaBoat/Proyect Wyrm
//push rax
//push rbx
//push rcx
//push rdx
//push rbp
//push rdi
//push rsi
//push r8
//push r9
//push r10
//push r11
//push r12
//push r13
//push r14
//push r15
//%endmacro
//
//%macro popState 0
//pop r15
//pop r14
//pop r13
//pop r12
//pop r11
//pop r10
//pop r9
//pop r8
//pop rsi
//pop rdi
//pop rbp
//pop rdx
//pop rcx
//pop rbx
//pop rax
//%endmacro


