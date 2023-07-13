#include "Vec.h"

__always_inline static I64 _capacity(const Vec * vec)
{
    return Arr_len(& vec->arr) - vec->index;
}

__always_inline static bool _no_capacity(const Vec * vec)
{
    return _capacity(vec) == 0;
}

__always_inline static void _double_capacity(Vec * vec)
{
    Arr_extend(& vec->arr, vec->index);
}

__always_inline void Vec_push_ptr(Vec * vec, const void * ptr)
{
    if (unlikely(_no_capacity(vec))) _double_capacity(vec);

    Vec_set_ptr(vec, vec->index ++, ptr);
}