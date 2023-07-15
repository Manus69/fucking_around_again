#include "Cstr.h"

#define DJB_INITIAL ((U64) 5381)
#define DJB_SHIFT   ((U64) 5)
static inline U64 _hash_djb(const char * cstr, I64 len)
{
    U64 hash;

    hash = DJB_INITIAL;

    while (len --)
    {
        hash = ((hash << DJB_SHIFT) + hash) + (U64)(* cstr ++);
    }

    return hash;
}

__always_inline U64 Cstr_hash_len(const Cstr cstr, I64 len)
{
    return _hash_djb(cstr, len);
}

U64 Cstr_hash(const void * ptr)
{
    return Cstr_hash_len(deref(Cstr) ptr, strlen(deref(Cstr) ptr));
}