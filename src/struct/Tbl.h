#ifndef TBL_H
#define TBL_H

#include "Vec.h"

typedef struct Tbl Tbl;

struct Tbl
{
    Arr rows;
};

static inline Tbl Tbl_init_Intr(const Intr * intr, I64 n_rows, I64 n_cols)
{
    Arr rows;

    rows = Arr_init(Vec, n_rows);
    for (I64 k = 0; k < n_rows; k ++)
    {
        deref(Vec) Arr_get(& rows, k) = Vec_init_intr(intr, n_cols);
    }

    return (Tbl) {rows};
}

__attribute__ ((pure))
static inline I64 Tbl_n_rows(const Tbl * tbl)
{
    return Arr_len(& tbl->rows);
}

__attribute__ ((pure, hot))
static inline Vec * Tbl_row(const Tbl * tbl, I64 n)
{
    return Arr_get(& tbl->rows, n);
}

__attribute__ ((pure))
static inline I64 Tbl_row_len(const Tbl * tbl, I64 row)
{
    return Vec_len(Tbl_row(tbl, row));
}

static inline const Intr * Tbl_item_intr(const Tbl * tbl)
{
    return Vec_item_intr(Arr_get(& tbl->rows, 0));
}

static inline void Tbl_map(const Tbl * tbl, F f)
{
    I64 n_rows;

    n_rows = Tbl_n_rows(tbl);
    for (I64 k = 0; k < n_rows; k ++)
    {
        Vec_map(Tbl_row(tbl, k), f);
    }
}

static inline void Tbl_del_struct(Tbl * tbl)
{
    Arr_map(& tbl->rows, Vec_del);
    Arr_del(& tbl->rows);
    to_zero(* tbl);
}

static inline void Tbl_del(void * ptr)
{
    Tbl_del_struct(ptr);
}

static inline void Tbl_del_items(Tbl * tbl)
{
    Tbl_map(tbl, Intr_del(Tbl_item_intr(tbl)));
}

__attribute__ ((pure, hot))
static inline void * Tbl_get(const Tbl * tbl, I64 row, I64 col)
{
    return Vec_get(Tbl_row(tbl, row), col);
}

static inline void Tbl_set_ptr(const Tbl * tbl, I64 row, I64 col, const void * ptr)
{
    Vec_set_ptr(Tbl_row(tbl, row), col, ptr);
}

static inline void Tbl_push_row(const Tbl * tbl, I64 row, const void * ptr)
{
    Vec_push_ptr(Tbl_row(tbl, row), ptr);
}

__attribute__ ((pure, flatten))
static inline I64 Tbl_find_in_row_cmp(const Tbl * tbl, I64 row, const void * ptr, Cmp cmp)
{
    return Vec_find_cmp(Tbl_row(tbl, row), ptr, cmp);
}

__attribute__ ((pure))
static inline bool Tbl_row_contains_cmp(const Tbl * tbl, I64 row, const void * ptr, Cmp cmp)
{
    return Tbl_find_in_row_cmp(tbl, row, ptr, cmp) != NO_INDEX;
}

#endif