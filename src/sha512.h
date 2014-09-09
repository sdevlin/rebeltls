#ifndef TLS_SHA512_H
#define TLS_SHA512_H

#include "types.h"

typedef struct {
  uint mlen;
  uint64 h[8];
  byte buf[128];
} sha512_ctx;

extern void sha512_init(sha512_ctx *ctx);
extern sha512_ctx *sha512_new(void);

extern void sha512_compress(sha512_ctx *ctx);

extern void sha512_update(sha512_ctx *ctx, const byte *m, uint mlen);
extern void sha512_final(sha512_ctx *ctx, byte *h);

extern void sha512_digest(const byte *m, uint mlen, byte *h);

#endif
