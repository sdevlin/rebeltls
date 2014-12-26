#include <string.h>
#include <stdlib.h>

#include "bc.h"
#include "log.h"
#include "inttypes.h"
#include "gcm.h"

/* static const u128 f = { */
/*   .hi = 0xe100000000000000, */
/*   .lo = 0x0000000000000000 */
/* }; */

/* static void gmul(u128 *x, const u128 *y, const u128 *z) */
/* { */
/*   uint i, c; */

/*   u128_zero(x); */

/*   for (i = 0; i < 128; i += 1) { */
/*     if (u128_bit(y, 0) == 1) { */
/*       u128_xor(x, x, z); */
/*     } */

/*     u128_rshift(y, 1); */
/*     c = u128_bit(z, 127); */
/*     u128_lshift(z, 1); */

/*     if (c == 1) { */
/*       u128_xor(z, z, &f); */
/*     } */
/*   } */
/* } */

static u8 zero[16];

void gcm_init(gcm_ctx *ctx, const bc_defn *defn,
              const u8 *key, const u8 *iv, int dir)
{
  log_assert(defn->keylen == 16, "bad keylen: %d", defn->keylen);

  ctx->defn = defn;
  defn->init(&ctx->cctx, key);
  defn->encrypt(&ctx->cctx, zero, ctx->h);
  memcpy(ctx->iv, iv, sizeof ctx->iv);
  ctx->ctr = 1;
  ctx->dir = dir;
}

gcm_ctx *gcm_new(const bc_defn *defn, const u8 *key,
                 const u8 *iv, int dir)
{
  gcm_ctx *ctx;

  ctx = malloc(sizeof *ctx);
  gcm_init(ctx, defn, key, iv, dir);
  return ctx;
}

void gcm_updatead(__attribute__ ((unused)) gcm_ctx *ctx,
                  __attribute__ ((unused)) const u8 *ad,
                  __attribute__ ((unused)) uint adlen)
{

}

void gcm_update(__attribute__ ((unused)) gcm_ctx *ctx,
                __attribute__ ((unused)) const u8 *in,
                __attribute__ ((unused)) u8 *out,
                __attribute__ ((unused)) uint len)
{

}

void gcm_final(__attribute__ ((unused)) gcm_ctx *ctx,
               __attribute__ ((unused)) u8 *tag)
{

}

void gcm_encrypt(const bc_defn *defn, const u8 *key,
                 const u8 *ad, uint adlen,
                 const u8 *p, uint plen,
                 u8 *c, u8 *tag)
{
  gcm_ctx ctx;

  gcm_init(&ctx, defn, key, NULL, GCM_ENCRYPT);
  gcm_updatead(&ctx, ad, adlen);
  gcm_update(&ctx, p, c, plen);
  gcm_final(&ctx, tag);
}

/* void gcm_decrypt(const bc_defn *defn, const u8 *key, */
/*                  const u8 *ad, uint adlen, */
/*                  const u8 *c, uint clen, */
/*                  u8 *p, const u8 *tag) */
/* { */

/* } */
