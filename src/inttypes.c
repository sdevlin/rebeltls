#include <assert.h>

#include "inttypes.h"

#define DEFINE_UINT(bits)                       \
  u##bits u##bits##_rotl(u##bits x, uint k)     \
  {                                             \
    assert(k > 0);                              \
    assert(k < bits);                           \
    return (x << k) | (x >> (bits-k));          \
  }                                             \
                                                \
  u##bits u##bits##_rotr(u##bits x, uint k)     \
  {                                             \
    assert(k > 0);                              \
    assert(k < bits);                           \
    return (x << (bits-k)) | (x >> k);          \
  }

DEFINE_UINT(8)
DEFINE_UINT(16)
DEFINE_UINT(32)
DEFINE_UINT(64)

u24 u24_clamp(u24 x)
{
  return x & 0xffffff;
}

void u128_zero(u128 *x)
{
  x->hi = 0;
  x->lo = 0;
}

void u128_xor(u128 *x, const u128 *y, const u128 *z)
{
  x->hi = y->hi ^ z->hi;
  x->lo = y->lo ^ z->lo;
}

uint u128_bit(u128 *x, uint k)
{
  u64 w;

  w = k < 64 ? x->lo : x->hi;
  k %= 64;
  return (w >> k) & 1;
}
