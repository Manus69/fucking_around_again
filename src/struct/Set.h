#ifndef SET_H
#define SET_H

#include "Vec.h"

#define Set_init(type) Set_init_Intr(& type##_Intr)
#define Set_insert(set_ptr, val) \
{typeof(val) _t = val; Set_insert_ptr(set_ptr, & _t);}

typedef struct Set Set;

struct Set
{
    Arr buckets;
    I64 n_items;
};

Set     Set_init_Intr_capacity(const Intr * intr, I64 capacity);
Set     Set_init_Intr(const Intr * intr);
bool    Set_contains_ptr(const Set * set, const void * item);
void    Set_del_struct(Set * set);
void    Set_del(void * ptr);
void *  Set_get_ptr(const Set * set, const void * item);
void *  Set_any(const Set * set);
void    Set_map(const Set * set, F f);
void    Set_del_items(const Set * set);
STATUS  Set_insert_ptr(Set * set, const void * item);
STATUS  Set_remove_ptr(void * target, Set * set, const void * item);

__always_inline static const Intr * Set_item_intr(const Set * set)
{
    return Vec_item_intr(Arr_get(& set->buckets, 0));
}

__always_inline static I64 Set_n_items(const Set * set)
{
    return set->n_items;
}

__always_inline static bool Set_empty(const Set * set)
{
    return set->n_items == 0;
}

static inline void Set_dbg(const void * set)
{
    Set_map(set, (F) Intr_dbg(Set_item_intr(set)));
}

#endif