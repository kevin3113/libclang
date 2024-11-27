#include <stdint.h>

struct __tiling__ TilingData
{
    int tile_cnt;
    int tile_size;
};

int add_custom(int a, int b, int c)
{
    TilingData t;
    int sum = a + b;
    int res = 0;
    t.tile_cnt = sum / c ;
    res = a + b * c;
    t.tile_size = c  * 8;
    return res;
}

