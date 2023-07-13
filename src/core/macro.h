#ifndef MACRO_H
#define MACRO_H

#define deref(type) * (type *)
#define likely(x)   (__builtin_expect((x), 1))
#define unlikely(x) (__builtin_expect((x), 0))
#define log2(n)     ((unsigned) (8 * sizeof (unsigned long long) - __builtin_clzll((n)) - 1))
#define is_pow_2(n) (__builtin_popcount((n)) == 1)
#define min(x, y)   ((x) < (y) ? (x) : (y))
#define max(x, y)   ((x) > (y) ? (x) : (y))
#define to_zero(val)(val = (typeof(val)) {0})

#endif