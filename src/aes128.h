#ifndef REBELTLS_AES128_H
#define REBELTLS_AES128_H

#include "inttypes.h"

enum {
  DIR_ENCRYPT,
  DIR_DECRYPT
};

typedef struct {
  int dir;
  uint32 w[44];
} aes128_ctx;

void aes128_init(aes128_ctx *ctx, const byte *key, int dir);
aes128_ctx *aes128_new(const byte *key, int dir);

void aes128_permute(const aes128_ctx *ctx, const byte *in, byte *out);

void aes128_encrypt(const byte *key, const byte *p, byte *c);
void aes128_decrypt(const byte *key, const byte *c, byte *p);

#endif
