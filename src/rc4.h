#ifndef REBELTLS_RC4_H
#define REBELTLS_RC4_H

#include "inttypes.h"

typedef struct {
  u8 i;
  u8 j;
  u8 S[256];
} rc4_ctx;

extern void rc4_init(rc4_ctx *ctx, const u8 *key, uint keylen);

extern void rc4_generate(rc4_ctx *ctx, u8 *out, uint outlen);

extern void rc4_update(rc4_ctx *ctx, const u8 *p, uint plen, u8 *c);
extern void rc4_final(rc4_ctx *ctx);

extern void rc4_encrypt(const u8 *key, uint keylen,
                        const u8 *p, uint plen, u8 *c);
extern void rc4_decrypt(const u8 *key, uint keylen,
                        const u8 *c, uint clen, u8 *p);

#endif
