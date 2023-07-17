#include "Ass.h"

#define ASS_DENSITY_THRESHOLD (3)
#define ASS_SCALE_F           (1 << 2)

typedef struct Idx Idx;

struct Idx
{
    U64 row;
    I64 col;
};

static inline bool _no_idx(const Idx * idx)
{
    return idx->col == NO_INDEX;
}

static inline Hash _k_hash(const Ass * ass)
{
    return Intr_hash(Ass_key_intr(ass));
}

static inline Hash _v_hash(const Ass * ass)
{
    return Intr_hash(Ass_val_intr(ass));
}

static inline Cmp _k_cmp(const Ass * ass)
{
    return Intr_cmp(Ass_key_intr(ass));
}

static inline Cmp _v_cmp(const Ass * ass)
{
    return Intr_cmp(Ass_val_intr(ass));
}

__attribute__ ((pure, hot, always_inline))
static inline U64 _n_rows(const Ass * ass)
{
    return Tbl_n_rows(& ass->keys);
}

__attribute__ ((pure, hot, always_inline))
static inline U64 _row_index(const Ass * ass, const void * key)
{
    return _k_hash(ass)(key) % _n_rows(ass);
}

__attribute__ ((pure, hot, always_inline))
static inline Idx _get_idx(const Ass * ass, const void * key)
{
    U64 row;
    I64 col;

    row = _row_index(ass, key);
    col = Tbl_find_in_row_cmp(& ass->keys, row, key, _k_cmp(ass));

    return (Idx) {row, col};
}

__attribute__ ((pure, hot))
void * Ass_get_ptr(const Ass * ass, const void * key)
{
    Idx idx;

    idx = _get_idx(ass, key);

    return _no_idx(& idx) ? NULL : Tbl_get(& ass->vals, idx.row, idx.col);
}

static inline void _add_key(Ass * ass, const void * key, U64 row)
{
    Tbl_push_row(& ass->keys, row, key);
}

static inline void _add_val(Ass * ass, const void * val, U64 row)
{
    Tbl_push_row(& ass->vals, row, val);
}

static inline void _change_val(Ass * ass, const void * val, Idx idx)
{
    Intr_put(Tbl_item_intr(& ass->vals))(Tbl_get(& ass->vals, idx.row, idx.col), val);
}

static inline void _change_val_buff(void * buff, Ass * ass, const void * val, Idx idx)
{
    Intr_put(Tbl_item_intr(& ass->vals))(buff, Tbl_get(& ass->vals, idx.row, idx.col));
    _change_val(ass, val, idx);
}

__attribute__ ((hot))
static inline void _insert(Ass * ass, const void * key, const void * val, U64 row)
{
    _add_key(ass, key, row);
    _add_val(ass, val, row);

    ++ ass->n_items;
}

static inline U64 _density(const Ass * ass)
{
    return ass->n_items / _n_rows(ass);
}

static inline void _rehash_row(Ass * target, Ass * ass, U64 row)
{
    U64     index;
    I64     len;
    void *  key;
    void *  val;

    len = Tbl_row_len(& ass->keys, row);
    for (I64 k = 0; k < len; k ++)
    {
        key = Vec_pop(Tbl_row(& ass->keys, row));
        val = Vec_pop(Tbl_row(& ass->vals, row));
        index = _row_index(target, key);
        _insert(target, key, val, index);
    }
}

static inline void _rebuild(Ass * ass)
{
    Ass new_ass;
    I64 n_rows;

    new_ass = Ass_init_Intr_capacity(Ass_key_intr(ass), Ass_val_intr(ass), _n_rows(ass) * ASS_SCALE_F);
    n_rows = _n_rows(ass);

    for (I64 k = 0; k < n_rows; k ++)
    {
        _rehash_row(& new_ass, ass, k);
    }

    Ass_del_struct(ass);
    * ass = new_ass;
}

__attribute__ ((hot))
ASS_INS_STATUS Ass_set_ptr_buff(void * buff, Ass * ass, const void * key, const void * val)
{
    Idx idx;

    if (unlikely(_density(ass) >= ASS_DENSITY_THRESHOLD)) _rebuild(ass);

    idx = _get_idx(ass, key);
    if (_no_idx(& idx))
    {
        _insert(ass, key, val, idx.row);
        return ASS_INS;
    }

    _change_val_buff(buff, ass, val, idx);

    return ASS_NOT_INS;
}

__attribute__ ((hot))
void Ass_set_ptr(Ass * ass, const void * key, const void * val)
{
    Idx idx;

    if (unlikely(_density(ass) >= ASS_DENSITY_THRESHOLD)) _rebuild(ass);

    idx = _get_idx(ass, key);
    if (_no_idx(& idx)) return _insert(ass, key, val, idx.row);

    _change_val(ass, val, idx);
}
