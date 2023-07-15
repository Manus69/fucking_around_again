#ifndef PAIR_H
#define PAIR_H

#include "../type/Intr.h"
#define Pair_set_first(pair_ptr, val) \
{typeof(val) _t = val; Pair_set_first_ptr(pair_ptr, & _t);}
#define Pair_set_second(pair_ptr, val) \
{typeof(val) _t = val; Pair_set_second_ptr(pair_ptr, & _t);}
#define Pair_set(pair_ptr, f, s) {Pair_set_first(pair_ptr, f); Pair_set_second(pair_ptr, s);}
#define Pair_init(f_type, s_type) Pair_init_Intr(& f_type##_Intr, & s_type##_Intr)

typedef struct Pair Pair;

struct Pair
{
    const Intr *    intr[2];
    void *          first;
    void *          second;
};

static inline Pair Pair_init_Intr(const Intr * first, const Intr * second)
{
    I64     size;
    void *  data;

    size = max(Intr_size(first), Intr_size(second));
    data = mem_allocate(size * 2);

    return (Pair) {{first, second}, data, data + size};
}

static inline void Pair_del_struct(Pair * pair)
{
    mem_del(pair->first);
    to_zero(* pair);
}

static inline void Pair_del(void * ptr)
{
    Pair_del_struct(ptr);
}

__always_inline static void * Pair_first(const Pair * pair)
{
    return pair->first;
}

__always_inline static void * Pair_second(const Pair * pair)
{
    return pair->second;
}

static inline void Pair_del_first(const Pair * pair)
{
    Intr_del(pair->intr[0])(pair->first);
}

static inline void Pair_del_second(const Pair * pair)
{
    Intr_del(pair->intr[1])(pair->second);
}

static inline void Pair_del_items(const Pair * pair)
{
    Pair_del_first(pair);
    Pair_del_second(pair);
}

static inline void Pair_erase(Pair * pair)
{
    Pair_del_items(pair);
    Pair_del_struct(pair);
}

__always_inline static void Pair_set_first_ptr(const Pair * pair, const void * ptr)
{
    Intr_put(pair->intr[0])(pair->first, ptr);
}

__always_inline static void Pair_set_second_ptr(const Pair * pair, const void * ptr)
{
    Intr_put(pair->intr[1])(pair->second, ptr);
}

__always_inline static void Pair_set_ptr(const Pair * pair, const void * first, const void * second)
{
    Pair_set_first(pair, first);
    Pair_set_second(pair, second);
}

#endif