#ifndef BUF_H
#define BUF_H

#include "../core/mem.h"

typedef struct Buf Buf;

struct Buf
{
    void *  bytes;
    I64     size;
};

static inline Buf Buf_init(I64 size)
{
    return (Buf) {mem_allocate(size), size};
}

static inline Buf Buf_init_zero(I64 size)
{
    return (Buf) {mem_allocate_zero(size), size};
}

static inline void Buf_del_struct(Buf * buf)
{
    mem_del(buf->bytes);
    to_zero(* buf);
}

static inline void Buf_del(void * ptr)
{
    Buf_del_struct(ptr);
}

__always_inline static I64 Buf_size(const Buf * buf)
{
    return buf->size;
}

__always_inline void * Buf_get(const Buf * buf, I64 index)
{
    return buf->bytes + index;
}

__always_inline void * Buf_bytes(const Buf * buf)
{
    return buf->bytes;
}

__always_inline void Buf_set(const Buf * buf, I64 index, U8 byte)
{
    deref(U8) Buf_get(buf, index) = byte;
}

static inline void Buf_set_many(const Buf * buf, I64 index, const void * ptr, I64 size)
{
    memcpy(Buf_get(buf, index), ptr, size);
}

static inline void Buf_extend(Buf * buf, I64 size)
{
    mem_extend(buf->bytes, buf->size, size);
    buf->size += size;
}

static inline void Buf_extend_zero(Buf * buf, I64 size)
{
    mem_extend_zero(buf->bytes, buf->size, size);
    buf->size += size;
}


#endif