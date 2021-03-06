#include "bindata.h"
#include "inttypes.h"
#include "md_defn.h"
#include "sha256.h"

const u32 sha256_initstate[] = {
  0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
  0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

static const u32 K[] = {
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

static u32 ch(u32 x, u32 y, u32 z)
{
  return (x & y) ^ (~x & z);
}

static u32 maj(u32 x, u32 y, u32 z)
{
  return (x & y) ^ (x & z) ^ (y & z);
}

static u32 S0(u32 x)
{
  return u32_rotr(x, 2) ^ u32_rotr(x, 13) ^ u32_rotr(x, 22);
}

static u32 S1(u32 x)
{
  return u32_rotr(x, 6) ^ u32_rotr(x, 11) ^ u32_rotr(x, 25);
}

static u32 s0(u32 x)
{
  return u32_rotr(x, 7) ^ u32_rotr(x, 18) ^ (x >> 3);
}

static u32 s1(u32 x)
{
  return u32_rotr(x, 17) ^ u32_rotr(x, 19) ^ (x >> 10);
}

void sha256_compress(sha256_ctx *ctx)
{
  int i;
  u32 w[64];
  u32 a, b, c, d, e, f, g, h;

  bindata_unpack(ctx->buf, "> LLLL LLLL LLLL LLLL",
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
    u32 T1, T2;

    T1 = h + S1(e) + ch(e, f, g) + K[i] + w[i];
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

static void packmlen(sha256_ctx *ctx)
{
  bindata_pack(ctx->buf + 56, "> Q", ctx->mlen << 3);
}

static void packh(sha256_ctx *ctx, u8 *h)
{
  bindata_pack(h, "> L[8]", ctx->h);
}

DEFINE_MD(sha256)
