#ifndef REBELTLS_SHA224_H
#define REBELTLS_SHA224_H

#include "sha256.h"
#include "inttypes.h"

typedef sha256_ctx sha224_ctx;

extern const uint32 sha224_initstate[8];

extern void sha224_init(sha224_ctx *ctx);
extern sha224_ctx *sha224_new(void);

extern void sha224_reset(sha224_ctx *ctx);

extern void sha224_compress(sha224_ctx *ctx);

extern void sha224_update(sha224_ctx *ctx, const uint8 *m, uint mlen);
extern void sha224_final(sha224_ctx *ctx, uint8 *h);

extern void sha224_digest(const uint8 *m, uint mlen, uint8 *h);

#endif
