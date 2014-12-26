#ifndef REBELTLS_SHA512_H
#define REBELTLS_SHA512_H

#include "inttypes.h"

typedef struct {
  uint mlen;
  uint64 h[8];
  uint8 buf[128];
} sha512_ctx;

extern const uint64 sha512_initstate[8];

extern void sha512_init(sha512_ctx *ctx);
extern sha512_ctx *sha512_new(void);

extern void sha512_compress(sha512_ctx *ctx);

extern void sha512_update(sha512_ctx *ctx, const uint8 *m, uint mlen);
extern void sha512_final(sha512_ctx *ctx, uint8 *h);

extern void sha512_digest(const uint8 *m, uint mlen, uint8 *h);

#endif
