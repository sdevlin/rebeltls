#ifndef TLS_HMAC_H
#define TLS_HMAC_H

#include "hash.h"
#include "types.h"

typedef struct {
  const hash_defn *hash_defn;
  hash_ctx ictx;
  hash_ctx octx;
  hash_ctx tmpctx;
} hmac_ctx;

extern void hmac_init(hmac_ctx *ctx, const hash_defn *defn,
                      const byte *key, uint keylen);
extern hmac_ctx *hmac_new(const hash_defn *defn,
                          const byte *key, uint keylen);

extern void hmac_reset(hmac_ctx *ctx);

extern void hmac_update(hmac_ctx *ctx, const byte *m, uint mlen);
extern void hmac_final(hmac_ctx *ctx, byte *h);

extern void hmac_digest(const hash_defn *defn,
                        const byte *key, uint keylen,
                        const byte *m, uint mlen,
                        byte *h);

#endif
