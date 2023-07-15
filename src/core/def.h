#ifndef DEF_H
#define DEF_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define OK              (1)
#define NOT_OK          (-1)
#define NO_INDEX        (-1)
#define BPB             (8)

typedef enum STATUS STATUS;

enum STATUS
{
    STATUS_OK = OK,
    STATUS_NOT_OK = NOT_OK,
};

typedef uint_fast8_t    U8;
typedef uint_fast64_t   U64;
typedef uint32_t        U32;
typedef int_fast64_t    I64;
typedef int_fast32_t    I32;
typedef void *          Ptr;
typedef char *          Cstr;
typedef void (* Put)    (void *, const void *);
typedef void (* Swap)   (void *, void *);
typedef void (* Del)    (void *);
typedef I64  (* Cmp)    (const void *, const void *);
typedef U64  (* Hash)   (const void *);
typedef void (* Dbg)    (const void *);
typedef void (* F)      (void *);

#endif