#include <string.h>

#include "haar.h"

void
haar_pass(int8_t* in, int8_t* ws, size_t len)
{
    size_t half_len = len >> 1;

    for (int idx=0; idx<half_len; idx++)
    {
        int lhs_idx = idx << 1;
        int16_t lhs = in[lhs_idx];

        int rhs_idx = lhs_idx + 1;
        int16_t rhs = in[rhs_idx];

        in[idx] = (lhs + rhs) >> 1;
        ws[idx] = (lhs - rhs) >> 1;
    }

    memcpy(in+half_len, ws, half_len);
}

static inline void
haar(int8_t* in, int8_t* ws, size_t len)
{
    for (size_t slen=len; slen >= 2; slen >>= 1)
        for (size_t offset=0; offset < len; offset += slen)
            haar_pass(in+offset, ws, slen);
}

void
haar4(int8_t* in)
{
    int16_t sum0 = in[0] + in[1];
    int16_t sum1 = in[2] + in[3];

    sum0 >>= 1;
    sum1 >>= 1;

    in[0] = (sum0 + sum1) >> 1;
    in[1] = (sum0 - sum1) >> 1;

    int16_t diff0 = in[0] - in[1];
    int16_t diff1 = in[2] - in[3];

    diff0 >>= 1;
    diff1 >>= 1;

    in[2] = (diff0 + diff1) >> 1;
    in[3] = (diff0 - diff1) >> 1;
}

void
haar8(int8_t* in, int8_t* ws)
{
    haar(in, ws, 8);
}

void
haar16(int8_t* in, int8_t* ws)
{
    haar(in, ws, 16);
}

void
haar32(int8_t* in, int8_t* ws)
{
    haar(in, ws, 32);
}

static inline void
loghaar(int8_t* in, int8_t* ws, size_t len)
{
    uint16_t start = 0;
    for (size_t slen=len; slen >= 2; slen >>= 1)
    {
        for (size_t offset=start; offset < len; offset += slen)
            haar_pass(in+offset, ws, slen);
        start += (slen >> 1);
    }
}

void
loghaar16(int8_t* in, int8_t* ws)
{
    haar(in, ws, 16);
}
