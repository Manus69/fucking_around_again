#ifndef SLC_H
#define SLC_H

#include "../core/mem.h"
#include "../type/Intr.h"
#include "../dbg/dbg.h"

#define Slc_set(slc_ptr, index, val) \
{typeof(val) _t = val; Slc_set_ptr(slc_ptr, index, & _t);}

typedef struct Slc Slc;

struct Slc
{
    void *          ptr;
    const Intr *    intr;
    I64             len;
};

static inline Slc Slc_init(void * ptr, const Intr * intr, I64 len)
{
    return (Slc) {ptr, intr, len};
}

static inline void Slc_del(Slc * slc)
{
    mem_del(slc->ptr);
    to_zero(* slc);
}

__always_inline static const Intr * Slc_item_intr(const Slc * slc)
{
    return slc->intr;
}

__always_inline static I64 Slc_item_size(const Slc * slc)
{
    return Intr_size(slc->intr);
}

__always_inline static I64 Slc_len(const Slc * slc)
{
    return slc->len;
}

__always_inline static I64 Slc_data_size(const Slc * slc)
{
    return Slc_len(slc) * Slc_item_size(slc);
}

static inline Slc Slc_dup(const Slc * slc)
{
    return Slc_init(mem_dup(slc->ptr, Slc_data_size(slc)), slc->intr, slc->len);
}

__always_inline static bool Slc_empty(const Slc * slc)
{
    return slc->len == 0;
}

__always_inline static void * Slc_ptr(const Slc * slc)
{
    return slc->ptr;
}

__always_inline static void * Slc_get(const Slc * slc, I64 index)
{
    return slc->ptr + index * Slc_item_size(slc);
}

__always_inline static void * Slc_first(const Slc * slc)
{
    return Slc_ptr(slc);
}

__always_inline static void * Slc_last(const Slc * slc)
{
    return Slc_get(slc, slc->len - 1);
}

__always_inline static void Slc_set_ptr(const Slc * slc, I64 index, const void * ptr)
{
    Intr_put(slc->intr)(Slc_get(slc, index), ptr);
}

__always_inline static void Slc_set_many(const Slc * slc, I64 index, const void * ptr, I64 n_items)
{
    mem_copy(Slc_get(slc, index), ptr, n_items, Slc_item_size(slc));
}

static inline void Slc_map(const Slc * slc, F f)
{
    mem_map(slc->ptr, slc->len, Intr_size(slc->intr), f);
}

static inline void Slc_del_items(const Slc * slc)
{
    if (Intr_del(slc->intr) == do_nothing) return ;

    Slc_map(slc, Intr_del(slc->intr));
}

static inline void Slc_dbg(const Slc * slc)
{
    Slc_map(slc, (F) Intr_dbg(Slc_item_intr((slc))));
    dbg_nl();
}

__always_inline static void Slc_shift(Slc * slc, I64 n_items)
{
    slc->ptr += n_items * Slc_item_size(slc);
    slc->len -= n_items;
}

__always_inline static void Slc_shrink(Slc * slc, I64 n_items)
{
    slc->len -= n_items;
}

static inline Slc Slc_slice(const Slc * slc, I64 index, I64 len)
{
    return Slc_init(Slc_get(slc, index), slc->intr, len);
}

static inline Slc Slc_to_Slc(const Slc * slc)
{
    return Slc_slice(slc, 0, slc->len);
}

__always_inline static Slc Slc_chop_front(Slc * slc, I64 len)
{
    Slc chop;

    chop = Slc_slice(slc, 0, len);
    Slc_shift(slc, len);

    return chop;
}

__always_inline static Slc Slc_chop_back(Slc * slc, I64 len)
{
    I64 index;
    Slc chop;

    index = slc->len - len;
    chop = Slc_slice(slc, index, len);
    Slc_shrink(slc, len);

    return chop;
}

__always_inline static Slc Slc_chop_all(Slc * slc)
{
    Slc chop;

    chop = Slc_to_Slc(slc);
    Slc_shift(slc, slc->len);

    return chop;
}

__always_inline static Slc Slc_chop_front_check(Slc * slc, I64 len)
{
    if (unlikely(len >= slc->len)) return Slc_chop_all(slc);

    return Slc_chop_front(slc, len);
}

__always_inline static I64 Slc_find_cmp(const Slc * slc, const void * item, Cmp cmp)
{
    for (I64 k = 0; k < slc->len; k ++)
    {
        if (equal(Slc_get(slc, k), item, cmp)) return k;
    }

    return NO_INDEX;
}

#define Slc_find_gen(type) \
__always_inline static I64 Slc_find##_##type(const Slc * slc, type val) \
{ \
    for (I64 k = 0; k < slc->len; k ++) \
    { \
        if (deref(type) Slc_get(slc, k) == val) return k; \
    } \
 \
    return NO_INDEX; \
}

#endif