#ifndef VEC_H
#define VEC_H

#include "Arr.h"
#include "Slc.h"

#define VEC_DC (1 << 5)
#define Vec_init(type) Vec_init_intr(& type##_Intr, VEC_DC)
#define Vec_push(vec_ptr, val) \
{typeof(val) _t = val; Vec_push_ptr(vec_ptr, & _t);}

typedef struct Vec Vec;

struct Vec
{
    Arr arr;
    I64 index;
};

void Vec_push_many(Vec * vec, const void * ptr, I64 n_items);
void Vec_push_ptr(Vec * vec, const void * ptr);
void Vec_reserve(Vec * vec, I64 capacity);

static inline Vec Vec_init_intr(const Intr * intr, I64 capacity)
{
    return (Vec) {Arr_init_intr(intr, capacity), 0};
}

static inline void Vec_del(Vec * vec)
{
    Arr_del(& vec->arr);
    to_zero(* vec);
}

__always_inline static const Intr * Vec_item_intr(const Vec * vec)
{
    return Arr_item_intr(& vec->arr);
}

__always_inline static I64 Vec_item_size(const Vec * vec)
{
    return Intr_size(Vec_item_intr(vec));
}

__always_inline static I64 Vec_len(const Vec * vec)
{
    return vec->index;
}

__always_inline static bool Vec_empty(const Vec * vec)
{
    return vec->index == 0;
}

__always_inline static void * Vec_get(const Vec * vec, I64 index)
{
    return Arr_get(& vec->arr, index);
}

__always_inline static void * Vec_first(const Vec * vec)
{
    return Vec_get(vec, 0);
}

__always_inline static void * Vec_last(const Vec * vec)
{
    return Vec_get(vec, vec->index - 1);
}

__always_inline static void Vec_set_ptr(const Vec * vec, I64 index, const void * ptr)
{
    Arr_set_ptr(& vec->arr, index, ptr);
}

__always_inline static void * Vec_pop(Vec * vec)
{
    return Vec_get(vec, -- vec->index);
}

static inline Slc Vec_slice(const Vec * vec, I64 index, I64 len)
{
    return Slc_init(Vec_get(vec, index), Vec_item_intr(vec), len);
}

static inline Slc Vec_to_Slc(const Vec * vec)
{
    return Vec_slice(vec, 0, Vec_len(vec));
}

#endif