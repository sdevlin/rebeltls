#ifndef REBELTLS_AES128_H
#define REBELTLS_AES128_H

#include "types.h"

typedef struct {
  uint32 w[44];
} aes128_ctx;

void aes128_init(aes128_ctx *ctx, const byte *key);

void aes128_encrypt(const aes128_ctx *ctx, const byte *p, byte *c);
void aes128_decrypt(const aes128_ctx *ctx, const byte *c, byte *p);

#endif
