#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "bytes.h"
#include "md.h"
#include "sha512.h"
#include "types.h"

static const uint64 h[] = {
  0x6a09e667f3bcc908, 0xbb67ae8584caa73b,
  0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1,
  0x510e527fade682d1, 0x9b05688c2b3e6c1f,
  0x1f83d9abfb41bd6b, 0x5be0cd19137e2179
};

void sha512_init(sha512_ctx *ctx)
{
  ctx->mlen = 0;
  memcpy(ctx->h, h, sizeof ctx->h);
}

sha512_ctx *sha512_new(void)
{
  sha512_ctx *ctx;
  ctx = malloc(sizeof *ctx);
  sha512_init(ctx);
  return ctx;
}

void sha512_reset(sha512_ctx *ctx)
{
  sha512_init(ctx);
}

static const uint64 K[] = {
  0x428a2f98d728ae22, 0x7137449123ef65cd,
  0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
  0x3956c25bf348b538, 0x59f111f1b605d019,
  0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
  0xd807aa98a3030242, 0x12835b0145706fbe,
  0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
  0x72be5d74f27b896f, 0x80deb1fe3b1696b1,
  0x9bdc06a725c71235, 0xc19bf174cf692694,
  0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
  0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
  0x2de92c6f592b0275, 0x4a7484aa6ea6e483,
  0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
  0x983e5152ee66dfab, 0xa831c66d2db43210,
  0xb00327c898fb213f, 0xbf597fc7beef0ee4,
  0xc6e00bf33da88fc2, 0xd5a79147930aa725,
  0x06ca6351e003826f, 0x142929670a0e6e70,
  0x27b70a8546d22ffc, 0x2e1b21385c26c926,
  0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
  0x650a73548baf63de, 0x766a0abb3c77b2a8,
  0x81c2c92e47edaee6, 0x92722c851482353b,
  0xa2bfe8a14cf10364, 0xa81a664bbc423001,
  0xc24b8b70d0f89791, 0xc76c51a30654be30,
  0xd192e819d6ef5218, 0xd69906245565a910,
  0xf40e35855771202a, 0x106aa07032bbd1b8,
  0x19a4c116b8d2d0c8, 0x1e376c085141ab53,
  0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
  0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
  0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
  0x748f82ee5defb2fc, 0x78a5636f43172f60,
  0x84c87814a1f0ab72, 0x8cc702081a6439ec,
  0x90befffa23631e28, 0xa4506cebde82bde9,
  0xbef9a3f7b2c67915, 0xc67178f2e372532b,
  0xca273eceea26619c, 0xd186b8c721c0c207,
  0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
  0x06f067aa72176fba, 0x0a637dc5a2c898a6,
  0x113f9804bef90dae, 0x1b710b35131c471b,
  0x28db77f523047d84, 0x32caab7b40c72493,
  0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
  0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
  0x5fcb6fab3ad6faec, 0x6c44198c4a475817
};

static uint64 rotr(uint64 x, uint n)
{
  assert(n > 0);
  assert(n < 64);
  return (x >> n) | (x << (64-n));
}

static uint64 ch(uint64 x, uint64 y, uint64 z)
{
  return (x & y) ^ (~x & z);
}

static uint64 maj(uint64 x, uint64 y, uint64 z)
{
  return (x & y) ^ (x & z) ^ (y & z);
}

static uint64 S0(uint64 x)
{
  return rotr(x, 28) ^ rotr(x, 34) ^ rotr(x, 39);
}

static uint64 S1(uint64 x)
{
  return rotr(x, 14) ^ rotr(x, 18) ^ rotr(x, 41);
}

static uint64 s0(uint64 x)
{
  return rotr(x, 1) ^ rotr(x, 8) ^ (x >> 7);
}

static uint64 s1(uint64 x)
{
  return rotr(x, 19) ^ rotr(x, 61) ^ (x >> 6);
}

void sha512_compress(sha512_ctx *ctx)
{
  int i;
  uint64 W[80];
  uint64 a, b, c, d, e, f, g, h;

  bytes_unpack(ctx->buf, "> QQQQ QQQQ QQQQ QQQQ",
               &W[0x0], &W[0x1], &W[0x2], &W[0x3],
               &W[0x4], &W[0x5], &W[0x6], &W[0x7],
               &W[0x8], &W[0x9], &W[0xa], &W[0xb],
               &W[0xc], &W[0xd], &W[0xe], &W[0xf]);

  for (i = 16; i < 80; i += 1) {
    W[i] = s1(W[i-2]) + W[i-7] + s0(W[i-15]) + W[i-16];
  }

  a = ctx->h[0];
  b = ctx->h[1];
  c = ctx->h[2];
  d = ctx->h[3];
  e = ctx->h[4];
  f = ctx->h[5];
  g = ctx->h[6];
  h = ctx->h[7];

  for (i = 0; i < 80; i += 1) {
    uint64 T1, T2;

    T1 = h + S1(e) + ch(e, f, g) + K[i] + W[i];
    T2 = S0(a) + maj(a, b, c);
    h = g;
    g = f;
    f = e;
    e = d + T1;
    d = c;
    c = b;
    b = a;
    a = T1 + T2;
  }

  ctx->h[0] += a;
  ctx->h[1] += b;
  ctx->h[2] += c;
  ctx->h[3] += d;
  ctx->h[4] += e;
  ctx->h[5] += f;
  ctx->h[6] += g;
  ctx->h[7] += h;
}

static byte *md_buffer(md_ctx *ctx)
{
  return ((sha512_ctx *)ctx)->buf;
}

static void md_compress(md_ctx *ctx)
{
  sha512_compress((sha512_ctx *)ctx);
}

static void md_packmlen(md_ctx *ctx)
{
  bytes_pack(md_buffer(ctx) + 112, "> QQ", 0, ctx->mlen << 3);
}

static const md_defn defn = {
  .buflen = 128,
  .buffer = &md_buffer,
  .compress = &md_compress,
  .mlenoffset = 112,
  .packmlen = &md_packmlen
};

void sha512_update(sha512_ctx *ctx, const byte *m, uint mlen)
{
  md_update(&defn, (md_ctx *)ctx, m, mlen);
}

void sha512_final(sha512_ctx *ctx, byte *h)
{
  md_final(&defn, (md_ctx *)ctx);
  bytes_pack(h, "> 8Q",
             ctx->h[0], ctx->h[1], ctx->h[2], ctx->h[3],
             ctx->h[4], ctx->h[5], ctx->h[6], ctx->h[7]);
}

void sha512_digest(const byte *m, uint mlen, byte *h)
{
  sha512_ctx ctx;

  sha512_init(&ctx);
  sha512_update(&ctx, m, mlen);
  sha512_final(&ctx, h);
}
