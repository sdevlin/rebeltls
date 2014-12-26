#ifndef REBELTLS_SHA256_H
#define REBELTLS_SHA256_H

#include "inttypes.h"

typedef struct {
  uint mlen;
  u32 h[8];
  u8 buf[64];
} sha256_ctx;

extern const u32 sha256_initstate[8];

extern void sha256_init(sha256_ctx *ctx);
extern sha256_ctx *sha256_new(void);

extern void sha256_reset(sha256_ctx *ctx);

extern void sha256_compress(sha256_ctx *ctx);

extern void sha256_update(sha256_ctx *ctx, const u8 *m, uint mlen);
extern void sha256_final(sha256_ctx *ctx, u8 *h);

extern void sha256_digest(const u8 *m, uint mlen, u8 *h);

#endif
