#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "bindata.h"
#include "md_defn.h"
#include "sha1.h"
#include "types.h"

const uint32 sha1_initstate[] = {
  0x67452301,
  0xefcdab89,
  0x98badcfe,
  0x10325476,
  0xc3d2e1f0
};

static const uint32 K[] = {
  0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999,
  0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999,
  0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999,
  0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999,
  0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1,
  0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1,
  0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1,
  0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1,
  0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc,
  0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc,
  0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc,
  0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc,
  0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6,
  0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6,
  0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6,
  0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6
};

static uint32 rotl(uint32 x, uint n)
{
  assert(n > 0);
  assert(n < 32);
  return (x << n) | (x >> (32-n));
}

static uint32 ch(uint32 x, uint32 y, uint32 z)
{
  return (x & y) ^ (~x & z);
}

static uint32 parity(uint32 x, uint32 y, uint32 z)
{
  return x ^ y ^ z;
}

static uint32 maj(uint32 x, uint32 y, uint32 z)
{
  return (x & y) ^ (x & z) ^ (y & z);
}

typedef uint32 (*sha1func)(uint32, uint32, uint32);

static const sha1func f[] = {
  &ch, &ch, &ch, &ch, &ch,
  &ch, &ch, &ch, &ch, &ch,
  &ch, &ch, &ch, &ch, &ch,
  &ch, &ch, &ch, &ch, &ch,
  &parity, &parity, &parity, &parity, &parity,
  &parity, &parity, &parity, &parity, &parity,
  &parity, &parity, &parity, &parity, &parity,
  &parity, &parity, &parity, &parity, &parity,
  &maj, &maj, &maj, &maj, &maj,
  &maj, &maj, &maj, &maj, &maj,
  &maj, &maj, &maj, &maj, &maj,
  &maj, &maj, &maj, &maj, &maj,
  &parity, &parity, &parity, &parity, &parity,
  &parity, &parity, &parity, &parity, &parity,
  &parity, &parity, &parity, &parity, &parity,
  &parity, &parity, &parity, &parity, &parity
};

void sha1_compress(sha1_ctx *ctx)
{
  int i;
  uint32 w[80];
  uint32 a, b, c, d, e;

  bindata_unpack(ctx->buf, "> LLLL LLLL LLLL LLLL",
                 &w[0x0], &w[0x1], &w[0x2], &w[0x3],
                 &w[0x4], &w[0x5], &w[0x6], &w[0x7],
                 &w[0x8], &w[0x9], &w[0xa], &w[0xb],
                 &w[0xc], &w[0xd], &w[0xe], &w[0xf]);

  for (i = 16; i < 80; i += 1) {
    w[i] = rotl((w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16]), 1);
  }

  a = ctx->h[0];
  b = ctx->h[1];
  c = ctx->h[2];
  d = ctx->h[3];
  e = ctx->h[4];

  for (i = 0; i < 80; i += 1) {
    uint32 T;

    T = rotl(a, 5) + f[i](b, c, d) + e + K[i] + w[i];
    e = d;
    d = c;
    c = rotl(b, 30);
    b = a;
    a = T;
  }

  ctx->h[0] += a;
  ctx->h[1] += b;
  ctx->h[2] += c;
  ctx->h[3] += d;
  ctx->h[4] += e;
}

static void packmlen(sha1_ctx *ctx)
{
  bindata_pack(ctx->buf + 56, "> Q", ctx->mlen << 3);
}

static void packh(sha1_ctx *ctx, byte *h)
{
  bindata_pack(h, "> L[5]", ctx->h);
}

MD_DEFN(sha1)
