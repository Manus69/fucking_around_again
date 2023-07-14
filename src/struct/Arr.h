#ifndef ARR_H
#define ARR_H

#include "../core/mem.h"
#include "../type/Intr.h"
#include "Slc.h"

#define Arr_init(type, len) Arr_init_intr(& type##_Intr, len)
#define Arr_set(arr_ptr, index, val) \
{typeof(val) _t = val; Arr_set_ptr(arr_ptr, index, & _t);}

typedef struct Arr Arr;

struct Arr
{
    void *          data;
    const Intr *    intr;
    I64             len;
};

static inline Arr Arr_init_intr(const Intr * intr, I64 len)
{
    return (Arr) {mem_allocate(len * Intr_size(intr)), intr, len};
}

__always_inline static I64 Arr_len(const Arr * arr)
{
    return arr->len;
}

static inline void Arr_del(Arr * arr)
{
    mem_del(arr->data);
    to_zero(* arr);
}

__always_inline static I64 Arr_item_size(const Arr * arr)
{
    return Intr_size(arr->intr);
}

__always_inline static I64 Arr_data_size(const Arr * arr)
{
    return Arr_len(arr) * Intr_size(arr->intr);
}

__always_inline static const Intr * Arr_item_intr(const Arr * arr)
{
    return arr->intr;
}

__always_inline static void * Arr_get(const Arr * arr, I64 index)
{
    return arr->data + index * Arr_item_size(arr);
}

__always_inline static void Arr_set_ptr(const Arr * arr, I64 index, const void * ptr)
{
    Intr_put(arr->intr)(Arr_get(arr, index), ptr);
}

static inline Slc Arr_slice(const Arr * arr, I64 index, I64 len)
{
    return Slc_init(Arr_get(arr, index), arr->intr, len);
}

static inline Slc Arr_to_Slc(const Arr * arr)
{
    return Arr_slice(arr, 0, Arr_len(arr));
}

static inline void Arr_map(const Arr * arr, F f)
{
    Slc slc;

    slc = Arr_to_Slc(arr);
    Slc_map(& slc, f);
}

static inline void Arr_dbg(const Arr * arr)
{
    Arr_map(arr, (F) Intr_dbg(arr->intr));
}

static inline void Arr_extend(Arr * arr, I64 len)
{
    arr->data = mem_extend(arr->data, Arr_data_size(arr), len * Arr_item_size(arr));
    arr->len += len;
}

#endif