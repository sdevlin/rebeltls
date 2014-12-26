#include <string.h>

#include "bytes.h"
#include "rc4.h"
#include "inttypes.h"

#define SWAP(x, y)                              \
  tmp = x;                                      \
  x = y;                                        \
  y = tmp;

void rc4_init(rc4_ctx *ctx, const u8 *key, uint keylen)
{
  uint i;
  u8 j, tmp;
  u8 *S;

  S = ctx->S;

  for (i = 0; i < 256; i += 1) {
    S[i] = i;
  }

  for (i = 0, j = 0; i < 256; i += 1) {
    j += S[i] + key[i%keylen];
    SWAP(S[i], S[j]);
  }

  ctx->i = 0;
  ctx->j = 0;
}

void rc4_generate(rc4_ctx *ctx, u8 *out, uint outlen)
{
  u8 i, j, tmp;
  u8 *S;

  i = ctx->i;
  j = ctx->j;
  S = ctx->S;

  for (; outlen > 0; out += 1, outlen -= 1) {
    i += 1;
    j += S[i];
    SWAP(S[i], S[j]);
    *out = S[(S[i] + S[j]) & 0xff];
  }

  ctx->i = i;
  ctx->j = j;
}

void rc4_update(rc4_ctx *ctx, const u8 *p, uint plen, u8 *c)
{
  rc4_generate(ctx, c, plen);
  bytes_xor(c, p, c, plen);
}

/* maybe unnecessary, sort of a placeholder */
void rc4_final(rc4_ctx *ctx)
{
  ctx->i = 0;
  ctx->j = 0;
  memset(ctx->S, 0, sizeof ctx->S);
}

void rc4_encrypt(const u8 *key, uint keylen,
                 const u8 *p, uint plen, u8 *c)
{
  rc4_ctx ctx;
  rc4_init(&ctx, key, keylen);
  rc4_update(&ctx, p, plen, c);
  rc4_final(&ctx);
}

void rc4_decrypt(const u8 *key, uint keylen,
                 const u8 *c, uint clen, u8 *p)
{
  rc4_encrypt(key, keylen, c, clen, p);
}
