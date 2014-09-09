#ifndef TLS_SHA1_H
#define TLS_SHA1_H

#include "types.h"

typedef struct {
  uint mlen;
  uint32 h[5];
  byte buf[64];
} sha1_ctx;

extern void sha1_init(sha1_ctx *ctx);
extern sha1_ctx *sha1_new(void);

extern void sha1_reset(sha1_ctx *ctx);

extern void sha1_compress(sha1_ctx *ctx);

extern void sha1_update(sha1_ctx *ctx, const byte *m, uint mlen);
extern void sha1_final(sha1_ctx *ctx, byte *h);

extern void sha1_digest(const byte *m, uint mlen, byte *h);

#endif
