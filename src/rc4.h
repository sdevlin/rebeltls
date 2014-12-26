#ifndef REBELTLS_RC4_H
#define REBELTLS_RC4_H

#include "inttypes.h"

typedef struct {
  uint8 i;
  uint8 j;
  uint8 S[256];
} rc4_ctx;

extern void rc4_init(rc4_ctx *ctx, const uint8 *key, uint keylen);

extern void rc4_generate(rc4_ctx *ctx, uint8 *out, uint outlen);

extern void rc4_update(rc4_ctx *ctx, const uint8 *p, uint plen, uint8 *c);
extern void rc4_final(rc4_ctx *ctx);

extern void rc4_encrypt(const uint8 *key, uint keylen,
                        const uint8 *p, uint plen, uint8 *c);
extern void rc4_decrypt(const uint8 *key, uint keylen,
                        const uint8 *c, uint clen, uint8 *p);

#endif
