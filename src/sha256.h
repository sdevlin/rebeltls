#ifndef TLS_SHA256_H
#define TLS_SHA256_H

#include "types.h"

typedef struct {
  uint mlen;
  uint32 h[8];
  byte buf[64];
} sha256_ctx;

extern void sha256_init(sha256_ctx *ctx);
extern sha256_ctx *sha256_new(void);

extern void sha256_reset(sha256_ctx *ctx);

extern void sha256_compress(sha256_ctx *ctx);

extern void sha256_update(sha256_ctx *ctx, const byte *m, uint mlen);
extern void sha256_final(sha256_ctx *ctx, byte *h);

extern void sha256_digest(const byte *m, uint mlen, byte *h);

#endif
