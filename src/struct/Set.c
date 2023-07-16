#include "Set.h"

#define SET_N_BUCKETS       (1 << 4)
#define SET_BUCKET_DC       (1 << 2)
#define DENSITY_THRESHOLD   (3)
#define SCALE_FACTOR        (1 << 2)

Set Set_init_Intr_capacity(const Intr * intr, I64 capacity)
{
    Arr buckets;

    buckets = Arr_init(Vec, capacity);
    for (I64 k = 0; k < capacity; k ++)
    {
        deref(Vec) Arr_get(& buckets, k) = Vec_init_intr(intr, SET_BUCKET_DC);
    }

    return (Set) {buckets, 0};
}

Set Set_init_Intr(const Intr * intr)
{
    return Set_init_Intr_capacity(intr, SET_N_BUCKETS);
}

static inline I64 _n(const Set * set)
{
    return Arr_len(& set->buckets);
}

__attribute__ ((flatten))
static inline I64 _index(const void * item, I64 n, Hash hash)
{
    return hash(item) % n;
}

static inline Vec * _bucket_by_index(const Set * set, I64 index)
{
    return Arr_get(& set->buckets, index);
}

__attribute__ ((flatten))
static inline Vec * _bucket(const Set * set, const void * item)
{
    I64 index;

    index = _index(item, _n(set), Intr_hash(Set_item_intr(set)));

    return _bucket_by_index(set, index);
}

bool Set_contains_ptr(const Set * set, const void * item)
{
    Vec * bucket;
    
    bucket = _bucket(set, item);

    return Vec_contains_cmp(bucket, item, Intr_cmp(Set_item_intr(set)));
}

static inline STATUS _insert(Set * set, Vec * bucket, const void * item)
{
    Vec_push_ptr(bucket, item);
    set->n_items ++;
    
    return STATUS_OK;
}

void Set_del_struct(Set * set)
{
    Arr_map(& set->buckets, Vec_del);
    Arr_del_struct(& set->buckets);
    to_zero(* set);
}

void Set_del(void * ptr)
{
    Set_del_struct(ptr);
}

void Set_map(const Set * set, F f)
{
    I64 n_buckets;

    n_buckets = _n(set);
    for (I64 k = 0; k < n_buckets; k ++)
    {
        Vec_map(_bucket_by_index(set, k), f);
    }
}

void Set_del_items(const Set * set)
{
    Set_map(set, Intr_del(Set_item_intr(set)));
}

static inline void _rehash_bucket(Set * target, const Vec * old_bucket)
{
    I64     len;
    Vec *   bucket;
    void *  item;

    len = Vec_len(old_bucket);
    for (I64 k = 0; k < len; k ++)
    {
        item = Vec_get(old_bucket, k);
        bucket = _bucket(target, item);
        _insert(target, bucket, item);
    }
}

static inline void _rehash(Set * set)
{
    Set new;
    I64 n;

    new = Set_init_Intr_capacity(Set_item_intr(set), _n(set) * SCALE_FACTOR);
    n = _n(set);

    for (I64 k = 0; k < n; k ++)
    {
        _rehash_bucket(& new, _bucket_by_index(set, k));
    }

    Set_del_struct(set);
    * set = new;
}

static inline I64 _density(const Set * set)
{
    return set->n_items / _n(set);
}

STATUS Set_insert_ptr(Set * set, const void * item)
{
    Vec * bucket;

    bucket = _bucket(set, item);
    if (Vec_contains_cmp(bucket, item, Intr_cmp(Set_item_intr(set)))) return STATUS_NOT_OK;
    if (unlikely(_density(set) >= DENSITY_THRESHOLD))
    {
        _rehash(set);
        bucket = _bucket(set, item);
    }

    return _insert(set, bucket, item);
}

void * Set_get_ptr(const Set * set, const void * item)
{
    Vec *   bucket;
    I64     index;

    bucket = _bucket(set, item);
    index = Vec_find_cmp(bucket, item, Intr_cmp(Set_item_intr(set)));

    return index == NO_INDEX ? NULL : Vec_get(bucket, index);
}

static inline Vec * _nonempty_bucket(const Set * set)
{
    Vec * bucket;
    
    for (I64 k = 0; ; k ++)
    {
        bucket = _bucket_by_index(set, k);
        if (Vec_empty(bucket) == false) return bucket;
    }
}

void * Set_any(const Set * set)
{
    if (Set_empty(set)) return NULL;

    return Vec_first(_nonempty_bucket(set));
}

// STATUS Set_remove_ptr(void * target, Set * set, const void * item)
// {
//     void * ptr;

//     if ((ptr = Set_get_ptr(set, item)) == NULL) return STATUS_NOT_OK;

//     Intr_put(Set_item_intr(set))(target, ptr);
//     _remove
//     -- set->n_items;

//     return STATUS_OK;
// }