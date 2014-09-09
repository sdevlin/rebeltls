#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "bytes.h"
#include "sha256.h"
#include "types.h"

static const uint32 h[] = {
  0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
  0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

void sha256_init(sha256_ctx *ctx)
{
  int i;
  ctx->mlen = 0;
  for (i = 0; i < 8; i += 1) {
    ctx->h[i] = h[i];
  }
}

sha256_ctx *sha256_new(void)
{
  sha256_ctx *ctx;
  ctx = malloc(sizeof *ctx);
  sha256_init(ctx);
  return ctx;
}

static const uint32 k[] = {
  0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
  0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
  0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
  0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
  0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
  0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
  0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
  0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
  0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
  0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
  0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
  0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static uint32 rotr(uint32 x, uint n)
{
  assert(n > 0);
  assert(n < 32);
  return (x >> n) | (x << (32-n));
}

static uint32 ch(uint32 x, uint32 y, uint32 z)
{
  return (x & y) ^ (~x & z);
}

static uint32 maj(uint32 x, uint32 y, uint32 z)
{
  return (x & y) ^ (x & z) ^ (y & z);
}

static uint32 S0(uint32 x)
{
  return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
}

static uint32 S1(uint32 x)
{
  return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
}

static uint32 s0(uint32 x)
{
  return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
}

static uint32 s1(uint32 x)
{
  return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
}

#include <unistd.h>

void sha256_compress(sha256_ctx *ctx)
{
  int i;
  uint32 w[64];
  uint32 a, b, c, d, e, f, g, h;

  bytes_unpack(ctx->buf, "> LLLL LLLL LLLL LLLL",
               &w[0x0], &w[0x1], &w[0x2], &w[0x3],
               &w[0x4], &w[0x5], &w[0x6], &w[0x7],
               &w[0x8], &w[0x9], &w[0xa], &w[0xb],
               &w[0xc], &w[0xd], &w[0xe], &w[0xf]);

  for (i = 16; i < 64; i += 1) {
    w[i] = s1(w[i-2]) + w[i-7] + s0(w[i-15]) + w[i-16];
  }

  a = ctx->h[0];
  b = ctx->h[1];
  c = ctx->h[2];
  d = ctx->h[3];
  e = ctx->h[4];
  f = ctx->h[5];
  g = ctx->h[6];
  h = ctx->h[7];

  for (i = 0; i < 64; i += 1) {
    uint32 T1, T2;

    T1 = h + S1(e) + ch(e, f, g) + k[i] + w[i];
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

static uint min(uint a, uint b)
{
  return a < b ? a : b;
}

void sha256_update(sha256_ctx *ctx, const byte *m, uint mlen)
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
      sha256_compress(ctx);
    }
  }
}

void sha256_final(sha256_ctx *ctx, byte *h)
{
  int i;

  i = ctx->mlen % 64;
  ctx->buf[i] = 0x80;
  i = (i + 1) % 64;

  for (; i != 56; i = (i + 1) % 64) {
    if (i == 0) {
      sha256_compress(ctx);
    }

    ctx->buf[i] = 0;
  }

  bytes_pack(ctx->buf + 56, "> Q", ctx->mlen << 3);

  sha256_compress(ctx);

  bytes_pack(h, "> 8L",
             ctx->h[0], ctx->h[1], ctx->h[2], ctx->h[3],
             ctx->h[4], ctx->h[5], ctx->h[6], ctx->h[7]);
}

void sha256_digest(const byte *m, uint mlen, byte *h)
{
  sha256_ctx ctx;

  sha256_init(&ctx);
  sha256_update(&ctx, m, mlen);
  sha256_final(&ctx, h);
}
