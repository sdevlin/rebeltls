#include "inttypes.h"

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
