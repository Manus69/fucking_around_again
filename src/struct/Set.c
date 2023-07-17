#include "Set.h"

#define SET_N_BUCKETS       (1 << 4)
#define SET_BUCKET_DC       (1 << 2)
#define DENSITY_THRESHOLD   (3)
#define SCALE_FACTOR        (1 << 2)

Set Set_init_Intr_capacity(const Intr * intr, I64 capacity)
{
    return (Set) {Tbl_init_Intr(intr, capacity, SET_BUCKET_DC), 0};
}

Set Set_init_Intr(const Intr * intr)
{
    return Set_init_Intr_capacity(intr, SET_N_BUCKETS);
}

__attribute__ ((pure))
static inline U64 _n_buckets(const Set * set)
{
    return Tbl_n_rows(& set->tbl);
}

__attribute__ ((pure, hot, flatten, always_inline))
static inline U64 _bucket_index(const Set * set, const void * item)
{
    return Intr_hash(Set_item_intr(set))(item) % _n_buckets(set);
}

__attribute__ ((pure, hot))
static inline Cmp _cmp(const Set * set)
{
    return Intr_cmp(Set_item_intr(set));
}

void Set_del_struct(Set * set)
{
    Tbl_del_struct(& set->tbl);
    to_zero(* set);
}

void Set_del(void * ptr)
{
    Set_del_struct(ptr);
}

void Set_map(const Set * set, F f)
{
    Tbl_map(& set->tbl, f);
}

void Set_del_items(const Set * set)
{
    Set_map(set, Intr_del(Set_item_intr(set)));
}

__attribute__ ((hot))
static inline STATUS _insert(Set * set, U64 index, const void * item)
{
    Tbl_push_row(& set->tbl, index, item);
    ++ set->n_items;

    return STATUS_OK;
}

static inline void _rehash_bucket(Set * target, const Vec * old_bucket)
{
    I64     len;
    void *  item;
    U64     index;

    len = Vec_len(old_bucket);
    for (I64 k = 0; k < len; k ++)
    {
        item = Vec_get(old_bucket, k);
        index = _bucket_index(target, item);
        _insert(target, index, item);
    }
}

static inline void _rehash(Set * set)
{
    Set new_set;
    U64 n;

    new_set = Set_init_Intr_capacity(Set_item_intr(set), _n_buckets(set) * SCALE_FACTOR);
    n = _n_buckets(set);

    for (U64 k = 0; k < n; k ++)
    {
        _rehash_bucket(& new_set, Tbl_row(& set->tbl, k));
    }

    Set_del_struct(set);
    * set = new_set;
}

static inline U64 _density(const Set * set)
{
    return set->n_items / _n_buckets(set);
}

__attribute__ ((hot))
STATUS Set_insert_ptr(Set * set, const void * item)
{
    U64 index;

    index = _bucket_index(set, item);
    if (Tbl_row_contains_cmp(& set->tbl, index, item, _cmp(set))) return STATUS_NOT_OK;
    if (unlikely(_density(set) >= DENSITY_THRESHOLD))
    {
        _rehash(set);
        index = _bucket_index(set, item);
    }

    return _insert(set, index, item);
}

__attribute__ ((pure, hot))
void * Set_get_ptr(const Set * set, const void * item)
{
    U64 index;
    I64 b_index;

    index = _bucket_index(set, item);
    b_index = Tbl_row_contains_cmp(& set->tbl, index, item, _cmp(set));

    return b_index == NO_INDEX ? NULL : Tbl_get(& set->tbl, index, b_index);
}

bool Set_contains_ptr(const Set * set, const void * item)
{
    return Set_get_ptr(set, item) != NULL;
}

static inline I64 _nonempty_bucket(const Set * set)
{
    for (I64 k = 0; ; k ++)
    {
        if (Tbl_row_len(& set->tbl, k)) return k;
    }
}

void * Set_any(const Set * set)
{
    if (Set_empty(set)) return NULL;

    return Tbl_get(& set->tbl, _nonempty_bucket(set), 0);
}

static inline STATUS _remove(void * buff, Set * set, const void * item)
{
    I64 index;
    I64 b_index;

    index = _bucket_index(set, item);
    b_index = Tbl_find_in_row_cmp(& set->tbl, index, item, _cmp(set));

    if (b_index == NO_INDEX) return STATUS_NOT_OK;
    if (buff) Vec_remove_index_buff(buff, Tbl_row(& set->tbl, index), b_index);
    else Vec_remove_index(Tbl_row(& set->tbl, index), b_index);

    return STATUS_OK;
}

STATUS Set_remove_ptr_buff(void * target, Set * set, const void * item)
{
    return _remove(target, set, item);
}

void Set_remove_ptr(Set * set, const void * item)
{
    _remove(NULL, set, item);
}