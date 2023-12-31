#ifndef MEM_H
#define MEM_H

#include "def.h"
#include "macro.h"

#include <assert.h>
#include <string.h>

void mem_vmap(F f, ...);

#define mem_put_val(ptr, val) deref(typeof(val)) ptr = val

#define mem_swap_t(lhs, rhs, type) \
{ \
    type _t = deref(type) (lhs); \
    deref(type) (lhs) = deref(type) (rhs); \
    deref(type) (rhs) = _t; \
}

#define mem_swap(lhs, rhs) mem_swap_t(lhs, rhs, typeof(* lhs))

#define mem_swap_gen(type) \
__attribute__ ((hot, nonnull)) \
__always_inline static void type##_swap(void * lhs, void * rhs) \
{ mem_swap_t(lhs, rhs, type); }

#define mem_put_gen(type) \
__attribute__ ((hot, nonnull)) \
__always_inline static void type##_put(void * target, const void * src) \
{ deref(type) target = deref(type) src; }

#define mem_cmp_gen(type) \
__attribute__ ((hot, pure, nonnull)) \
__always_inline static I64 type##_cmp(const void * lhs, const void * rhs) \
{ return deref(type) lhs < deref(type) rhs ? -1 : deref(type) lhs > deref(type) rhs;}

#define mem_del_gen_noop(type) \
__always_inline static void type##_del(void * ptr) {do_nothing(ptr);}

__attribute__ ((returns_nonnull, hot))
static inline void * mem_allocate(I64 size)
{
    void * ptr;

    if ((ptr = malloc(size))) return ptr;

    assert(0);
    return NULL;
}

__attribute__ ((returns_nonnull, hot))
static inline void * mem_allocate_zero(I64 size)
{
    void * ptr;

    if ((ptr = calloc(size, 1))) return ptr;

    assert(0);
    return NULL;
}

static inline void mem_del(void * ptr)
{
    free(ptr);
}

__attribute__ ((nonnull, returns_nonnull, hot))
static inline void * mem_reallocate(void * ptr, I64 size)
{
    void * new_ptr;

    if ((new_ptr = realloc(ptr, size))) return new_ptr;

    assert(0);
    return NULL;
}

__attribute__ ((returns_nonnull, hot))
static inline void * mem_extend(void * ptr, I64 current_size, I64 extra_size)
{
    return mem_reallocate(ptr, current_size + extra_size);
}

__attribute__ ((returns_nonnull))
static inline void * mem_extend_zero(void * ptr, I64 current_size, I64 extra_size)
{
    void * _ptr;

    _ptr = mem_extend(ptr, current_size, extra_size);
    memset(_ptr + current_size, 0, extra_size);

    return _ptr;
}

static inline void mem_map(void * data, I64 n_items, I64 item_size, F f)
{
    while (n_items --)
    {
        f(data);
        data += item_size;
    }
}

static inline void mem_copy(void * restrict dst, const void * restrict src, I64 n_items, I64 item_size)
{
    memcpy(dst, src, n_items * item_size);
}

static inline void mem_move(void * dst, const void * src, I64 n_items, I64 item_size)
{
    I64 size;

    size = n_items * item_size;
    memmove(dst, src, size);
}

static inline void * mem_dup(const void * ptr, I64 size)
{
    void * new_ptr;

    new_ptr = mem_allocate(size);
    return memcpy(new_ptr, ptr, size);
}

static inline void mem_set_zero(void * ptr, I64 n_items, I64 item_size)
{
    memset(ptr, 0, n_items * item_size);
}

__always_inline static void do_nothing(void * ptr)
{
    (void) ptr;
}

__attribute__ ((flatten, pure))
__always_inline static bool equal(const void * lhs, const void * rhs, Cmp cmp)
{
    return cmp(lhs, rhs) == 0;
}

#endif