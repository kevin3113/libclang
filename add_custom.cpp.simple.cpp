#include <stdint.h>
#include "pre_def.h"
#include "add.h"

struct __tiling__ TilingData
{
    int tile_cnt;
    int tile_size;
};

template <int x=1, typename T>
__global__ __aicore__ void add_custom(__gm__ uint8_t *a, __gm__ uint8_t *b, int c, T &s)
{
    TilingData t;
    t.tile_cnt = GET_SHAPE(a)[0];
    t.tile_size = GET_SHAPE(b)[0];
    s = (T)(*a + *b + c);
    s *= x;
    s += t.tile_cnt * t.tile_size;
}

//template void add_custom<10, unsigned long>(int a, int b, int c, unsigned long &s, TilingData &t);
//template <int x=1, typename T>
//__global__ __aicore__ void add_custom(int a, int b, int c, T &s, TilingData *t);
int main(void)
{
    long z;
    uint8_t a = 0;
    uint8_t b = 1;
    add_custom<2, long>(&a, &b, 3, z);
}
