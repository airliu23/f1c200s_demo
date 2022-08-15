#ifndef __TYPES_H__
#define __TYPES_H__

typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;

typedef signed char     int8;
typedef signed short    int16;
typedef signed int      int32;

typedef enum {
    false = 0,
    true = !false,
}bool;

#define NULL    ((void *)0)

#define __IO    volatile



#endif
