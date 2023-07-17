#include "sort.h"

__always_inline static void _drop(Slc * restrict target, Slc * restrict src)
{
    Slc_set_ptr(target, 0, Slc_first(src));
    Slc_shift(target, 1);
    Slc_shift(src, 1);
}

__always_inline static void _dump(Slc * restrict target, Slc * restrict src)
{
    Slc_set_many(target, 0, Slc_first(src), Slc_len(src));
    Slc_shift(target, Slc_len(src));
    Slc_shift(src, Slc_len(src));
}

__always_inline static void _dump_all(Slc * restrict target, Slc * restrict lhs, Slc * restrict rhs)
{
    _dump(target, lhs);
    _dump(target, rhs);
}

__always_inline static bool _is_sorted(const Slc * restrict lhs, const Slc * restrict rhs, Cmp cmp)
{
    return Slc_empty(rhs) || (cmp(Slc_last(lhs), Slc_first(rhs)) <= 0);
}

__attribute__ ((flatten, nonnull))
static inline void _merge(Slc * restrict target, Slc * restrict lhs, Slc * restrict rhs, Cmp cmp)
{
    if (unlikely(_is_sorted(lhs, rhs, cmp))) _dump_all(target, lhs, rhs);

    while (true)
    {
        if (Slc_empty(rhs)) return _dump(target, lhs);
        if (Slc_empty(lhs)) return _dump(target, rhs);

        if (cmp(Slc_first(lhs), Slc_first(rhs)) < 0) _drop(target, lhs);
        else _drop(target, rhs);
    }
}

static inline void _pass(Slc * target, Slc * src, I64 frame, Cmp cmp)
{
    Slc lhs;
    Slc rhs;

    while (Slc_empty(src) == false)
    {
        lhs = Slc_chop_front_check(src, frame);
        rhs = Slc_chop_front_check(src, frame);

        _merge(target, & lhs, & rhs, cmp);
    }
}

static inline void _swap_rewind(Slc * restrict target, Slc * restrict src, I64 len)
{
    Slc_shift(target, - len);
    Slc_shift(src, - len);
    mem_swap(& target->ptr, & src->ptr);
}

static inline I64 _preprocess_insert(const Slc * slc, I64 frame, Cmp cmp)
{
    Slc _slc;
    Slc current;

    _slc = Slc_to_Slc(slc);
    while (Slc_empty(& _slc) == false)
    {
        current = Slc_chop_front_check(& _slc, frame);
        sort_insert_slice_cmp(& current, cmp);
    }

    return frame;
}

static inline I64 _preprocess_swap(Slc * slc, Cmp cmp)
{
    I64 len;
    Ptr lhs;
    Ptr rhs;

    len = Slc_len(slc);
    for (I64 index = 1; index < len; index += 2)
    {
        lhs = Slc_get(slc, index - 1);
        rhs = Slc_get(slc, index);

        if (cmp(lhs, rhs) > 0) Intr_swap(Slc_item_intr(slc))(lhs, rhs);
    }

    return 2;
}

#define PREPROCESS_FRAME (1 << 3)
static inline I64 _preprocess(Slc * slc, Cmp cmp)
{
    return _preprocess_insert(slc, PREPROCESS_FRAME, cmp);
    // return _preprocess_swap(slc, cmp);
}

__attribute__ ((nonnull))
void sort_merge_slice_cmp(Slc * slc, Cmp cmp)
{
    I64 frame;
    I64 len;
    I64 swaps;
    Slc target;

    swaps = 0;
    target = Slc_dup(slc);
    len = Slc_len(slc);
    frame = _preprocess(slc, cmp);

    while (frame <= len)
    {
        _pass(& target, slc, frame, cmp);
        _swap_rewind(& target, slc, len);

        swaps ++;
        frame <<= 1;
    }
    
    if (swaps % 2) mem_swap(& target.ptr, & slc->ptr);

    Slc_del(& target);
}
