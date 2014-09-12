#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "bindata.h"
#include "md.h"
#include "md5.h"
#include "types.h"

static const uint32 H[] = {
  0x67452301,
  0xefcdab89,
  0x98badcfe,
  0x10325476
};

void md5_init(md5_ctx *ctx)
{
  ctx->mlen = 0;
  memcpy(ctx->h, H, sizeof ctx->h);
}

md5_ctx *md5_new(void)
{
  md5_ctx *ctx;
  ctx = malloc(sizeof *ctx);
  md5_init(ctx);
  return ctx;
}

void md5_reset(md5_ctx *ctx)
{
  md5_init(ctx);
}

static const uint32 T[] = {
  0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
  0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
  0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
  0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
  0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
  0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
  0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
  0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
  0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
  0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
  0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
  0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
  0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
  0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
  0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
  0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

static uint32 rotl(uint32 x, uint n)
{
  assert(n > 0);
  assert(n < 32);
  return (x << n) | (x >> (32-n));
}

static uint32 f(uint32 x, uint32 y, uint32 z)
{
  return (x & y) | (~x & z);
}

static uint32 g(uint32 x, uint32 y, uint32 z)
{
  return (x & z) | (y & ~z);
}

static uint32 h(uint32 x, uint32 y, uint32 z)
{
  return x ^ y ^ z;
}

static uint32 i(uint32 x, uint32 y, uint32 z)
{
  return y ^ (x | ~z);
}

#define OP(a, b, c, d, f, k, s, i)                \
  a = b + rotl((a + f(b, c, d) + x[k] + T[i]), s)

void md5_compress(md5_ctx *ctx)
{
  uint32 x[16];
  uint32 a, b, c, d;

  /* TODO change when unpack is done */
  bindata_unpack(ctx->buf, "< LLLL LLLL LLLL LLLL",
                 &x[0x0], &x[0x1], &x[0x2], &x[0x3],
                 &x[0x4], &x[0x5], &x[0x6], &x[0x7],
                 &x[0x8], &x[0x9], &x[0xa], &x[0xb],
                 &x[0xc], &x[0xd], &x[0xe], &x[0xf]);

  a = ctx->h[0];
  b = ctx->h[1];
  c = ctx->h[2];
  d = ctx->h[3];

  OP(a, b, c, d, f, 0x0, 0x07, 0x00);
  OP(d, a, b, c, f, 0x1, 0x0c, 0x01);
  OP(c, d, a, b, f, 0x2, 0x11, 0x02);
  OP(b, c, d, a, f, 0x3, 0x16, 0x03);
  OP(a, b, c, d, f, 0x4, 0x07, 0x04);
  OP(d, a, b, c, f, 0x5, 0x0c, 0x05);
  OP(c, d, a, b, f, 0x6, 0x11, 0x06);
  OP(b, c, d, a, f, 0x7, 0x16, 0x07);
  OP(a, b, c, d, f, 0x8, 0x07, 0x08);
  OP(d, a, b, c, f, 0x9, 0x0c, 0x09);
  OP(c, d, a, b, f, 0xa, 0x11, 0x0a);
  OP(b, c, d, a, f, 0xb, 0x16, 0x0b);
  OP(a, b, c, d, f, 0xc, 0x07, 0x0c);
  OP(d, a, b, c, f, 0xd, 0x0c, 0x0d);
  OP(c, d, a, b, f, 0xe, 0x11, 0x0e);
  OP(b, c, d, a, f, 0xf, 0x16, 0x0f);

  OP(a, b, c, d, g, 0x1, 0x05, 0x10);
  OP(d, a, b, c, g, 0x6, 0x09, 0x11);
  OP(c, d, a, b, g, 0xb, 0x0e, 0x12);
  OP(b, c, d, a, g, 0x0, 0x14, 0x13);
  OP(a, b, c, d, g, 0x5, 0x05, 0x14);
  OP(d, a, b, c, g, 0xa, 0x09, 0x15);
  OP(c, d, a, b, g, 0xf, 0x0e, 0x16);
  OP(b, c, d, a, g, 0x4, 0x14, 0x17);
  OP(a, b, c, d, g, 0x9, 0x05, 0x18);
  OP(d, a, b, c, g, 0xe, 0x09, 0x19);
  OP(c, d, a, b, g, 0x3, 0x0e, 0x1a);
  OP(b, c, d, a, g, 0x8, 0x14, 0x1b);
  OP(a, b, c, d, g, 0xd, 0x05, 0x1c);
  OP(d, a, b, c, g, 0x2, 0x09, 0x1d);
  OP(c, d, a, b, g, 0x7, 0x0e, 0x1e);
  OP(b, c, d, a, g, 0xc, 0x14, 0x1f);

  OP(a, b, c, d, h, 0x5, 0x04, 0x20);
  OP(d, a, b, c, h, 0x8, 0x0b, 0x21);
  OP(c, d, a, b, h, 0xb, 0x10, 0x22);
  OP(b, c, d, a, h, 0xe, 0x17, 0x23);
  OP(a, b, c, d, h, 0x1, 0x04, 0x24);
  OP(d, a, b, c, h, 0x4, 0x0b, 0x25);
  OP(c, d, a, b, h, 0x7, 0x10, 0x26);
  OP(b, c, d, a, h, 0xa, 0x17, 0x27);
  OP(a, b, c, d, h, 0xd, 0x04, 0x28);
  OP(d, a, b, c, h, 0x0, 0x0b, 0x29);
  OP(c, d, a, b, h, 0x3, 0x10, 0x2a);
  OP(b, c, d, a, h, 0x6, 0x17, 0x2b);
  OP(a, b, c, d, h, 0x9, 0x04, 0x2c);
  OP(d, a, b, c, h, 0xc, 0x0b, 0x2d);
  OP(c, d, a, b, h, 0xf, 0x10, 0x2e);
  OP(b, c, d, a, h, 0x2, 0x17, 0x2f);

  OP(a, b, c, d, i, 0x0, 0x06, 0x30);
  OP(d, a, b, c, i, 0x7, 0x0a, 0x31);
  OP(c, d, a, b, i, 0xe, 0x0f, 0x32);
  OP(b, c, d, a, i, 0x5, 0x15, 0x33);
  OP(a, b, c, d, i, 0xc, 0x06, 0x34);
  OP(d, a, b, c, i, 0x3, 0x0a, 0x35);
  OP(c, d, a, b, i, 0xa, 0x0f, 0x36);
  OP(b, c, d, a, i, 0x1, 0x15, 0x37);
  OP(a, b, c, d, i, 0x8, 0x06, 0x38);
  OP(d, a, b, c, i, 0xf, 0x0a, 0x39);
  OP(c, d, a, b, i, 0x6, 0x0f, 0x3a);
  OP(b, c, d, a, i, 0xd, 0x15, 0x3b);
  OP(a, b, c, d, i, 0x4, 0x06, 0x3c);
  OP(d, a, b, c, i, 0xb, 0x0a, 0x3d);
  OP(c, d, a, b, i, 0x2, 0x0f, 0x3e);
  OP(b, c, d, a, i, 0x9, 0x15, 0x3f);

  ctx->h[0] += a;
  ctx->h[1] += b;
  ctx->h[2] += c;
  ctx->h[3] += d;
}

static byte *md_buffer(md_ctx *ctx)
{
  return ((md5_ctx *)ctx)->buf;
}

static void md_compress(md_ctx *ctx)
{
  md5_compress((md5_ctx *)ctx);
}

static void md_packmlen(md_ctx *ctx)
{
  bindata_pack(md_buffer(ctx) + 56, "< Q", ctx->mlen << 3);
}

static const md_defn defn = {
  .buflen = 64,
  .buffer = &md_buffer,
  .compress = &md_compress,
  .mlenoffset = 56,
  .packmlen = &md_packmlen
};

void md5_update(md5_ctx *ctx, const byte *m, uint mlen)
{
  md_update(&defn, (md_ctx *)ctx, m, mlen);
}

void md5_final(md5_ctx *ctx, byte *h)
{
  md_final(&defn, (md_ctx *)ctx);
  bindata_pack(h, "< L[4]", ctx->h);
}

void md5_digest(const byte *m, uint mlen, byte *h)
{
  md5_ctx ctx;

  md5_init(&ctx);
  md5_update(&ctx, m, mlen);
  md5_final(&ctx, h);
}