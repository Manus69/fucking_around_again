#ifndef DBG_H
#define DBG_H

#include <stdio.h>

#define dbg_gen(type, format) \
static inline void type##_dbg(const void * ptr) \
{printf(format, deref(type) ptr);}

static inline void dbg_nl()
{
    printf("\n");
}

#endif