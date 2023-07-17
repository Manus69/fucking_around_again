#ifndef SET_H
#define SET_H

#include "Tbl.h"

#define Set_init(type) Set_init_Intr(& type##_Intr)
#define Set_insert(set_ptr, val) \
{typeof(val) _t = val; Set_insert_ptr(set_ptr, & _t);}
#define Set_remove(set_ptr, val) \
{typeof(val) _t = val; Set_remove_ptr(set_ptr, & _t);}

typedef struct Set Set;

struct Set
{
    Tbl tbl;
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
void    Set_remove_ptr(Set * set, const void * item);
STATUS  Set_insert_ptr(Set * set, const void * item);
STATUS  Set_remove_ptr_buff(void * target, Set * set, const void * item);


static inline const Intr * Set_item_intr(const Set * set)
{
    return Tbl_item_intr(& set->tbl);
}

__attribute__ ((pure))
static inline I64 Set_n_items(const Set * set)
{
    return set->n_items;
}

__attribute__ ((pure))
static inline bool Set_empty(const Set * set)
{
    return set->n_items == 0;
}

static inline void Set_dbg(const void * set)
{
    Set_map(set, (F) Intr_dbg(Set_item_intr(set)));
    dbg_nl();
}

#endif