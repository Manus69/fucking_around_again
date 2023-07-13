#ifndef VEC_H
#define VEC_H

#include "Arr.h"

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

void Vec_push_ptr(Vec * vec, const void * ptr);

static inline Vec Vec_init_intr(const Intr * intr, I64 capacity)
{
    return (Vec) {Arr_init_intr(intr, capacity), 0};
}

static inline void Vec_del(Vec * vec)
{
    Arr_del(& vec->arr);
    to_zero(* vec);
}

__always_inline static I64 Vec_len(const Vec * vec)
{
    return vec->index;
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



#endif