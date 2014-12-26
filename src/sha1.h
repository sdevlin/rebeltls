#ifndef REBELTLS_SHA1_H
#define REBELTLS_SHA1_H

#include "inttypes.h"

typedef struct {
  uint mlen;
  u32 h[5];
  u8 buf[64];
} sha1_ctx;

extern const u32 sha1_initstate[5];

extern void sha1_init(sha1_ctx *ctx);
extern sha1_ctx *sha1_new(void);

extern void sha1_reset(sha1_ctx *ctx);

extern void sha1_compress(sha1_ctx *ctx);

extern void sha1_update(sha1_ctx *ctx, const u8 *m, uint mlen);
extern void sha1_final(sha1_ctx *ctx, u8 *h);

extern void sha1_digest(const u8 *m, uint mlen, u8 *h);

#endif
