#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "hmac.h"
#include "bytes.h"

static const byte ipad[] = {
  0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
  0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
  0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
  0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
  0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
  0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
  0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
  0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
  0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
  0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
  0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
  0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
  0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
  0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
  0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
  0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36
};

/* ipad ^ opad */
static const byte iopad[] = {
  0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a,
  0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a,
  0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a,
  0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a,
  0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a,
  0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a,
  0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a,
  0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a,
  0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a,
  0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a,
  0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a,
  0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a,
  0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a,
  0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a,
  0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a,
  0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a
};

void hmac_init(hmac_ctx *ctx, const hash_defn *defn,
               const byte *key, uint keylen)
{
  byte k[HASH_MAX_BLOCKLEN];

  assert(sizeof ipad >= defn->blocklen);
  assert(sizeof iopad >= defn->blocklen);

  ctx->hash_defn = defn;
  defn->init(&ctx->ictx);
  defn->init(&ctx->octx);

  if (keylen > defn->blocklen) {
    defn->digest(key, keylen, k);
    memset(k + defn->hashlen, 0, defn->blocklen - defn->hashlen);
  } else {
    memcpy(k, key, keylen);
    memset(k + keylen, 0, defn->blocklen - keylen);
  }

  bytes_xor(k, k, ipad, defn->blocklen);
  defn->update(&ctx->ictx, k, defn->blocklen);

  bytes_xor(k, k, iopad, defn->blocklen);
  defn->update(&ctx->octx, k, defn->blocklen);

  ctx->tmpctx = ctx->ictx;
}

hmac_ctx *hmac_new(const hash_defn *defn, const byte *key, uint keylen)
{
  hmac_ctx *ctx;
  ctx = malloc(sizeof *ctx);
  hmac_init(ctx, defn, key, keylen);
  return ctx;
}

void hmac_reset(hmac_ctx *ctx)
{
  ctx->tmpctx = ctx->ictx;
}

void hmac_update(hmac_ctx *ctx, const byte *m, uint mlen)
{
  ctx->hash_defn->update(&ctx->tmpctx, m, mlen);
}

void hmac_final(hmac_ctx *ctx, byte *h)
{
  ctx->hash_defn->final(&ctx->tmpctx, h);
  ctx->tmpctx = ctx->octx;
  ctx->hash_defn->update(&ctx->tmpctx, h, ctx->hash_defn->hashlen);
  ctx->hash_defn->final(&ctx->tmpctx, h);
}

void hmac_digest(const hash_defn *defn, const byte *key, uint keylen,
                 const byte *m, uint mlen, byte *h)
{
  hmac_ctx ctx;
  hmac_init(&ctx, defn, key, keylen);
  hmac_update(&ctx, m, mlen);
  hmac_final(&ctx, h);
}
