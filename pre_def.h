#pragma once

#include <stdint.h>

#define __global__ __attribute__((annotate("__global__")))
#define __aicore__ __attribute__((annotate("__aicore__")))
#define __gm__ __attribute__((annotate("__gm__")))
#define __ubuf__ __attribute__((annotate("__ubuf__")))
#define __tiling__ __attribute__((annotate("__tiling__")))
#define GM_ADDR __gm__ uint8_t *


int shape_a[] = {8};
int shape_b[] = {16};
#define GET_SHAPE(x) shape_##x
