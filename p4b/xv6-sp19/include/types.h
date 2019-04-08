#ifndef _TYPES_H_
#define _TYPES_H_

// Type definitions

typedef struct __lock_t 
{
    int ticket;
    int turn;
} lock_t;

typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pde_t;
#ifndef NULL
#define NULL (0)
#endif

#endif //_TYPES_H_
