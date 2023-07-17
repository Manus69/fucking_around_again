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

__attribute__ ((hot, nonnull))
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

static inline void _remove(Vec * vec, I64 index)
{
    if (index == vec->index - 1) return (void) Vec_pop(vec);

    memmove(Vec_get(vec, index), Vec_get(vec, index + 1), (vec->index - index) * Vec_item_size(vec));
    -- vec->index;
}

void Vec_remove_index_buff(void * target, Vec * vec, I64 index)
{
    Intr_put(Vec_item_intr(vec))(target, Vec_get(vec, index));

    _remove(vec, index);
}

void Vec_remove_index(Vec * vec, I64 index)
{
    _remove(vec, index);
}

STATUS Vec_remove_item_buff(void * target, Vec * vec, const void * item)
{
    I64 index;

    index = Vec_find_cmp(vec, item, Intr_cmp(Vec_item_intr(vec)));
    if (index == NO_INDEX) return STATUS_NOT_OK;

    Vec_remove_index_buff(target, vec, index);

    return STATUS_OK;
}