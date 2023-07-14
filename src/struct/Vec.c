#include "Vec.h"

__always_inline static I64 _capacity(const Vec * vec)
{
    return Arr_len(& vec->arr) - vec->index;
}

__always_inline static bool _no_capacity(const Vec * vec)
{
    return _capacity(vec) == 0;
}

__always_inline static void _extend(Vec * vec, I64 n_items)
{
    Arr_extend(& vec->arr, n_items);
}

__always_inline static void _double_capacity(Vec * vec)
{
    _extend(vec, vec->index);
}

__always_inline void Vec_push_ptr(Vec * vec, const void * ptr)
{
    if (unlikely(_no_capacity(vec))) _double_capacity(vec);

    Vec_set_ptr(vec, vec->index ++, ptr);
}

void Vec_push_many(Vec * vec, const void * ptr, I64 n_items)
{
    Vec_reserve(vec, n_items);

    mem_copy(Vec_get(vec, vec->index), ptr, n_items, Vec_item_size(vec));
    vec->index += n_items;
}

void Vec_reserve(Vec * vec, I64 capacity)
{
    if (_capacity(vec) >= capacity) return;

    _extend(vec, capacity - _capacity(vec));
}