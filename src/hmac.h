#ifndef REBELTLS_HMAC_H
#define REBELTLS_HMAC_H

#include "hash.h"
#include "inttypes.h"

typedef struct {
  const hash_desc *hash_desc;
  hash_ctx ictx;
  hash_ctx octx;
  hash_ctx tmpctx;
} hmac_ctx;

extern void hmac_init(hmac_ctx *ctx, const hash_desc *desc,
                      const uint8 *key, uint keylen);
extern hmac_ctx *hmac_new(const hash_desc *desc,
                          const uint8 *key, uint keylen);

extern void hmac_reset(hmac_ctx *ctx);

extern void hmac_update(hmac_ctx *ctx, const uint8 *m, uint mlen);
extern void hmac_final(hmac_ctx *ctx, uint8 *h);

extern void hmac_digest(const hash_desc *desc,
                        const uint8 *key, uint keylen,
                        const uint8 *m, uint mlen,
                        uint8 *h);

#endif
