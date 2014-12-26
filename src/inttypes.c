#include <assert.h>

#include "inttypes.h"

#define DEFINE_UINT(name)                                   \
  static const uint name##_bitsize = (sizeof (name)) << 3;  \
                                                            \
  name name##_min(name x, name y)                           \
  {                                                         \
    return x < y ? x : y;                                   \
  }                                                         \
                                                            \
  name name##_max(name x, name y)                           \
  {                                                         \
    return x > y ? x : y;                                   \
  }                                                         \
                                                            \
  name name##_rotl(name x, uint k)                          \
  {                                                         \
    assert(k > 0);                                          \
    assert(k < name##_bitsize);                             \
    return (x << k) | (x >> (name##_bitsize-k));            \
  }                                                         \
                                                            \
  name name##_rotr(name x, uint k)                          \
  {                                                         \
    assert(k > 0);                                          \
    assert(k < name##_bitsize);                             \
    return (x << (name##_bitsize-k)) | (x >> k);            \
  }

DEFINE_UINT(uint)
DEFINE_UINT(u8)
DEFINE_UINT(u16)
DEFINE_UINT(u32)
DEFINE_UINT(u64)

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
