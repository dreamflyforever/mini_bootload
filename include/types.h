#ifndef __ASM_ARM_TYPES_H
#define __ASM_ARM_TYPES_H

typedef unsigned short umode_t;

/*
 * These aren't exported outside the kernel to avoid name space clashes
 */


typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef signed long long s64;
typedef unsigned long long u64;


/* Dma addresses are 32-bits wide.  */

typedef u32 dma_addr_t;

typedef unsigned long phys_addr_t;
typedef unsigned long phys_size_t;

/*else*/
typedef unsigned int uint;


#endif
