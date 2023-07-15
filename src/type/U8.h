#ifndef U8_H
#define U8_H

#include "Intr.h"

mem_put_gen(U8)
mem_swap_gen(U8)
mem_cmp_gen(U8)
dbg_gen(U8, "%u")

__always_inline static U64 U8_hash(const void * ptr)
{
    return deref(U8) ptr;
}

Intr_gen(U8)

#endif