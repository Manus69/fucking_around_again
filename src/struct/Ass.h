#ifndef ASS_H
#define ASS_H

#include "Tbl.h"
#define ASS_DC          (1 << 4)
#define ASS_BUCKET_DC   (1 << 2)

#define Ass_init(k_t, v_t) Ass_init_Intr(& k_t##_Intr, & v_t##_Intr)
#define Ass_set(ass_ptr, k, v) \
{typeof(k) _k = k; typeof(v) _v = v; Ass_set_ptr(ass_ptr, & _k, & _v);}

typedef enum ASS_INS_STATUS ASS_INS_STATUS;
typedef struct Ass Ass;

enum ASS_INS_STATUS {ASS_INS, ASS_NOT_INS};

struct Ass
{
    Tbl keys;
    Tbl vals;
    I64 n_items;
};

void *          Ass_get_ptr(const Ass * ass, const void * key);
ASS_INS_STATUS  Ass_set_ptr_buff(void * buff, Ass * ass, const void * key, const void * val);
void            Ass_set_ptr(Ass * ass, const void * key, const void * val);

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

static inline void Ass_del_struct(Ass * ass)
{
    Tbl_del_struct(& ass->keys);
    Tbl_del_struct(& ass->vals);
    to_zero(* ass);
}

static inline void Ass_del_items(Ass * ass)
{
    Tbl_del_items(& ass->keys);
    Tbl_del_items(& ass->vals);
}

static inline void Ass_del(void * ptr)
{
    Ass_del_struct(ptr);
}

static inline void Ass_erase(Ass * ass)
{
    Ass_del_items(ass);
    Ass_del_struct(ass);
}

static inline void Ass_dbg(const void * ptr)
{
    Ass * ass;

    ass = (Ass *) ptr; 
    for (I64 row = 0; row < Tbl_n_rows(& ass->keys); row ++)
    {
        printf("{ ");
        for (I64 col = 0; col < Tbl_row_len(& ass->keys, row); col ++)
        {
            Intr_dbg(Ass_key_intr(ass))(Tbl_get(& ass->keys, row, col));
            printf(" : ");
            Intr_dbg(Ass_val_intr(ass))(Tbl_get(& ass->vals, row, col));
        }
        printf(" }");
    }
    printf("\n");
}

#endif