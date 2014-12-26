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

void aes128_init(aes128_ctx *ctx, const uint8 *key, int dir);
aes128_ctx *aes128_new(const uint8 *key, int dir);

void aes128_permute(const aes128_ctx *ctx, const uint8 *in, uint8 *out);

void aes128_encrypt(const uint8 *key, const uint8 *p, uint8 *c);
void aes128_decrypt(const uint8 *key, const uint8 *c, uint8 *p);

#endif
