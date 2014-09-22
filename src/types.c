#include "types.h"

uint24 uint24_clamp(uint24 x)
{
  return x & 0xffffff;
}

void uint128_zero(uint128 *x)
{
  x->hi = 0;
  x->lo = 0;
}

void uint128_xor(uint128 *x, const uint128 *y, const uint128 *z)
{
  x->hi = y->hi ^ z->hi;
  x->lo = y->lo ^ z->lo;
}

uint uint128_bit(uint128 *x, uint k)
{
  uint64 w;

  w = k < 64 ? x->lo : x->hi;
  k %= 64;
  return (w >> k) & 1;
}
