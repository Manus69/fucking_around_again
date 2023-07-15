#ifndef CSTR_H
#define CSTR_H

#include "Intr.h"

U64 Cstr_hash_len(const Cstr cstr, I64 len);
U64 Cstr_hash(const void * ptr);

__always_inline static I64 Cstr_cmp(const void * lhs, const void * rhs)
{
    return strcmp(deref(Cstr) lhs, deref(Cstr) rhs);
}

__always_inline static void Cstr_del(void * ptr)
{
    do_nothing(ptr);
}

mem_put_gen(Cstr)
mem_swap_gen(Cstr)
dbg_gen(Cstr, "%s\n")

Intr_gen(Cstr)

#endif