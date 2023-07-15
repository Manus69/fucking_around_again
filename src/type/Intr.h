#ifndef INTR_H
#define INTR_H

#include "../core/mem.h"
#include "../dbg/dbg.h"

#define Intr_gen(type) \
static const Intr type##_Intr = (Intr) \
{ \
    type##_put, \
    type##_swap, \
    type##_del, \
    type##_cmp, \
    type##_hash, \
    type##_dbg, \
    sizeof(type), \
};

typedef struct Intr Intr;

struct Intr
{
    Put     put;
    Swap    swap;
    Del     del;
    Cmp     cmp;
    Hash    hash;
    Dbg     dbg;
    I64     size;
};

__always_inline static Put Intr_put(const Intr * intr)
{
    return intr->put;
}

__always_inline static Swap Intr_swap(const Intr * intr)
{
    return intr->swap;
}

__always_inline static Del Intr_del(const Intr * intr)
{
    return intr->del;
}

__always_inline static Cmp Intr_cmp(const Intr * intr)
{
    return intr->cmp;
}

__always_inline static Hash Intr_hash(const Intr * intr)
{
    return intr->hash;
}

__always_inline static Dbg Intr_dbg(const Intr * intr)
{
    return intr->dbg;
}

__always_inline static I64 Intr_size(const Intr * intr)
{
    return intr->size;
}

#endif