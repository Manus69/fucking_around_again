#ifndef I64_H
#define I64_H

#include "Intr.h"

mem_put_gen(I64)
mem_swap_gen(I64)
mem_cmp_gen(I64)
dbg_gen(I64, "%ld ")

__always_inline static U64 I64_hash(const void * ptr)
{
    return deref(I64) ptr;
}

Intr_gen(I64)

#endif