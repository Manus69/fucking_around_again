#ifndef PAIR_H
#define PAIR_H

#include "../type/Intr.h"

typedef struct Pair Pair;

struct Pair
{
    Intr * intr[2];
    void * first;
    void * second;
};

static inline Pair Pair_init_Intr(const Intr * first, const Intr * second)
{
    
}

#endif