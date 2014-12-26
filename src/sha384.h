#ifndef REBELTLS_SHA384_H
#define REBELTLS_SHA384_H

#include "sha512.h"
#include "inttypes.h"

typedef sha512_ctx sha384_ctx;

extern void sha384_init(sha384_ctx *ctx);
extern sha384_ctx *sha384_new(void);

extern void sha384_compress(sha384_ctx *ctx);

extern void sha384_update(sha384_ctx *ctx, const uint8 *m, uint mlen);
extern void sha384_final(sha384_ctx *ctx, uint8 *h);

extern void sha384_digest(const uint8 *m, uint mlen, uint8 *h);

#endif
