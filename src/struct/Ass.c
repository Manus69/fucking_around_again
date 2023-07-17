#include "Ass.h"

typedef struct Idx Idx;

struct Idx
{
    U64 row;
    I64 col;
};

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
static inline U64 _n_buckets(const Ass * ass)
{
    return Tbl_n_rows(& ass->keys);
}

__attribute__ ((pure, hot, always_inline))
static inline U64 _bucket_index(const Ass * ass, const void * key)
{
    return _k_hash(ass)(key) % _n_buckets(ass);
}

__attribute__ ((pure, hot, always_inline))
static inline Idx _get_idx(const Ass * ass, const void * key)
{
    U64 row;
    I64 col;

    row = _bucket_index(ass, key);
    col = Tbl_find_in_row_cmp(& ass->keys, row, key, _k_cmp);

    return (Idx) {row, col};
}

void * Ass_get_ptr(const Ass * ass, const void * key)
{

}