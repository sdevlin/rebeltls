#ifndef REBELTLS_SHA224_H
#define REBELTLS_SHA224_H

#include "sha256.h"
#include "inttypes.h"

typedef sha256_ctx sha224_ctx;

extern const u32 sha224_initstate[8];

extern void sha224_init(sha224_ctx *ctx);
extern sha224_ctx *sha224_new(void);

extern void sha224_reset(sha224_ctx *ctx);

extern void sha224_compress(sha224_ctx *ctx);

extern void sha224_update(sha224_ctx *ctx, const u8 *m, uint mlen);
extern void sha224_final(sha224_ctx *ctx, u8 *h);

extern void sha224_digest(const u8 *m, uint mlen, u8 *h);

#endif
