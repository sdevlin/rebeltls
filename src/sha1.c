#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "bytes.h"
#include "sha1.h"
#include "types.h"

static const uint32 h[] = {
  0x67452301,
  0xefcdab89,
  0x98badcfe,
  0x10325476,
  0xc3d2e1f0
};

void sha1_init(sha1_ctx *ctx)
{
  int i;
  ctx->mlen = 0;
  for (i = 0; i < 5; i += 1) {
    ctx->h[i] = h[i];
  }
}

sha1_ctx *sha1_new(void)
{
  sha1_ctx *ctx;
  ctx = malloc(sizeof *ctx);
  sha1_init(ctx);
  return ctx;
}

static const uint32 k[] = {
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

  bytes_unpack(ctx->buf, "> LLLL LLLL LLLL LLLL",
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

    T = rotl(a, 5) + f[i](b, c, d) + e + k[i] + w[i];
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

static uint min(uint a, uint b)
{
  return a < b ? a : b;
}

void sha1_update(sha1_ctx *ctx, const byte *m, uint mlen)
{
  uint offset, take;

  while (mlen > 0) {
    offset = ctx->mlen % 64;
    take = min(64 - offset, mlen);

    memcpy(ctx->buf + offset, m, take);
    ctx->mlen += take;

    m += take;
    mlen -= take;

    if (ctx->mlen % 64 == 0) {
      sha1_compress(ctx);
    }
  }
}

void sha1_final(sha1_ctx *ctx, byte *h)
{
  int i;

  i = ctx->mlen % 64;
  ctx->buf[i] = 0x80;
  i = (i + 1) % 64;

  for (; i != 56; i = (i + 1) % 64) {
    if (i == 0) {
      sha1_compress(ctx);
    }

    ctx->buf[i] = 0;
  }

  bytes_pack(ctx->buf + 56, "> Q", ctx->mlen << 3);

  sha1_compress(ctx);

  bytes_pack(h, "> 5L",
             ctx->h[0], ctx->h[1], ctx->h[2], ctx->h[3], ctx->h[4]);
}

void sha1_digest(const byte *m, uint mlen, byte *h)
{
  sha1_ctx ctx;

  sha1_init(&ctx);
  sha1_update(&ctx, m, mlen);
  sha1_final(&ctx, h);
}
