#ifndef CHAR_H
#define CHAR_H

#include "Intr.h"

mem_put_gen(char)
mem_swap_gen(char)
mem_cmp_gen(char)
dbg_gen(char, "%c ");

__always_inline static U64 char_hash(const void * ptr)
{
    return deref(char) ptr;
}

Intr_gen(char)

#endif