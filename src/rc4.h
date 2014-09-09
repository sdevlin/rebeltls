#ifndef TLS_RC4_H
#define TLS_RC4_H

#include "types.h"

typedef struct {
  uint8 i;
  uint8 j;
  uint8 S[256];
} rc4_ctx;

extern void rc4_init(rc4_ctx *ctx, const byte *key, uint keylen);

extern void rc4_generate(rc4_ctx *ctx, byte *out, uint outlen);

extern void rc4_update(rc4_ctx *ctx, const byte *p, uint plen, byte *c);
extern void rc4_final(rc4_ctx *ctx);

extern void rc4_encrypt(const byte *key, uint keylen,
                        const byte *p, uint plen, byte *c);
extern void rc4_decrypt(const byte *key, uint keylen,
                        const byte *c, uint clen, byte *p);

#endif
