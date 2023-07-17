#ifndef ASS_H
#define ASS_H

#include "Tbl.h"
#define ASS_DC          (1 << 4)
#define ASS_BUCKET_DC   (1 << 2)

typedef struct Ass Ass;

struct Ass
{
    Tbl keys;
    Tbl vals;
    I64 n_items;
};

static inline Ass Ass_init_Intr_capacity(const Intr * k_intr, const Intr * v_intr, I64 capacity)
{
    return (Ass) {Tbl_init_Intr(k_intr, capacity, ASS_BUCKET_DC), 
                  Tbl_init_Intr(v_intr, capacity, ASS_BUCKET_DC),
                  0};
}

static inline Ass Ass_init_Intr(const Intr * k_intr, const Intr * v_intr)
{
    return Ass_init_Intr_capacity(k_intr, v_intr, ASS_DC);
}

static inline I64 Ass_n_items(const Ass * ass)
{
    return ass->n_items;
}

__attribute__ ((pure, hot))
static inline const Intr * Ass_key_intr(const Ass * ass)
{
    return Tbl_item_intr(& ass->keys);
}

__attribute__ ((pure, hot))
static inline const Intr * Ass_val_intr(const Ass * ass)
{
    return Tbl_item_intr(& ass->vals);
}

#endif