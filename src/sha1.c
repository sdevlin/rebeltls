#include "bindata.h"
#include "inttypes.h"
#include "md_defn.h"
#include "sha1.h"

const u32 sha1_initstate[] = {
  0x67452301,
  0xefcdab89,
  0x98badcfe,
  0x10325476,
  0xc3d2e1f0
};

static const u32 K[] = {
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

static u32 ch(u32 x, u32 y, u32 z)
{
  return (x & y) ^ (~x & z);
}

static u32 parity(u32 x, u32 y, u32 z)
{
  return x ^ y ^ z;
}

static u32 maj(u32 x, u32 y, u32 z)
{
  return (x & y) ^ (x & z) ^ (y & z);
}

typedef u32 (*sha1func)(u32, u32, u32);

#define TIMES20(x) x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x

static const sha1func f[] = {
  TIMES20(&ch),
  TIMES20(&parity),
  TIMES20(&maj),
  TIMES20(&parity)
};

void sha1_compress(sha1_ctx *ctx)
{
  int i;
  u32 w[80];
  u32 a, b, c, d, e;

  bindata_unpack(ctx->buf, "> LLLL LLLL LLLL LLLL",
                 &w[0x0], &w[0x1], &w[0x2], &w[0x3],
                 &w[0x4], &w[0x5], &w[0x6], &w[0x7],
                 &w[0x8], &w[0x9], &w[0xa], &w[0xb],
                 &w[0xc], &w[0xd], &w[0xe], &w[0xf]);

  for (i = 16; i < 80; i += 1) {
    w[i] = u32_rotl((w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16]), 1);
  }

  a = ctx->h[0];
  b = ctx->h[1];
  c = ctx->h[2];
  d = ctx->h[3];
  e = ctx->h[4];

  for (i = 0; i < 80; i += 1) {
    u32 T;

    T = u32_rotl(a, 5) + f[i](b, c, d) + e + K[i] + w[i];
    e = d;
    d = c;
    c = u32_rotl(b, 30);
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

static void packh(sha1_ctx *ctx, u8 *h)
{
  bindata_pack(h, "> L[5]", ctx->h);
}

DEFINE_MD(sha1)
