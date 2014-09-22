#include <string.h>
#include <stdlib.h>

#include "bc.h"
#include "log.h"
#include "types.h"
#include "gcm.h"

/* static const uint128 f = { */
/*   .hi = 0xe100000000000000, */
/*   .lo = 0x0000000000000000 */
/* }; */

/* static void gmul(uint128 *x, const uint128 *y, const uint128 *z) */
/* { */
/*   uint i, c; */

/*   uint128_zero(x); */

/*   for (i = 0; i < 128; i += 1) { */
/*     if (uint128_bit(y, 0) == 1) { */
/*       uint128_xor(x, x, z); */
/*     } */

/*     uint128_rshift(y, 1); */
/*     c = uint128_bit(z, 127); */
/*     uint128_lshift(z, 1); */

/*     if (c == 1) { */
/*       uint128_xor(z, z, &f); */
/*     } */
/*   } */
/* } */

static byte zero[16];

void gcm_init(gcm_ctx *ctx, const bc_defn *defn,
              const byte *key, const byte *iv, int dir)
{
  log_assert(defn->keylen == 16, "bad keylen: %d", defn->keylen);

  ctx->defn = defn;
  defn->init(&ctx->cctx, key);
  defn->encrypt(&ctx->cctx, zero, ctx->h);
  memcpy(ctx->iv, iv, sizeof ctx->iv);
  ctx->ctr = 1;
  ctx->dir = dir;
}

gcm_ctx *gcm_new(const bc_defn *defn, const byte *key,
                 const byte *iv, int dir)
{
  gcm_ctx *ctx;

  ctx = malloc(sizeof *ctx);
  gcm_init(ctx, defn, key, iv, dir);
  return ctx;
}

void gcm_updatead(__attribute__ ((unused)) gcm_ctx *ctx,
                  __attribute__ ((unused)) const byte *ad,
                  __attribute__ ((unused)) uint adlen)
{

}

void gcm_update(__attribute__ ((unused)) gcm_ctx *ctx,
                __attribute__ ((unused)) const byte *in,
                __attribute__ ((unused)) byte *out,
                __attribute__ ((unused)) uint len)
{

}

void gcm_final(__attribute__ ((unused)) gcm_ctx *ctx,
               __attribute__ ((unused)) byte *tag)
{

}

void gcm_encrypt(const bc_defn *defn, const byte *key,
                 const byte *ad, uint adlen,
                 const byte *p, uint plen,
                 byte *c, byte *tag)
{
  gcm_ctx ctx;

  gcm_init(&ctx, defn, key, NULL, GCM_ENCRYPT);
  gcm_updatead(&ctx, ad, adlen);
  gcm_update(&ctx, p, c, plen);
  gcm_final(&ctx, tag);
}

/* void gcm_decrypt(const bc_defn *defn, const byte *key, */
/*                  const byte *ad, uint adlen, */
/*                  const byte *c, uint clen, */
/*                  byte *p, const byte *tag) */
/* { */

/* } */
