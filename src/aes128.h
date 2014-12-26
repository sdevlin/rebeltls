#ifndef REBELTLS_AES128_H
#define REBELTLS_AES128_H

#include "inttypes.h"

enum {
  DIR_ENCRYPT,
  DIR_DECRYPT
};

typedef struct {
  int dir;
  u32 w[44];
} aes128_ctx;

void aes128_init(aes128_ctx *ctx, const u8 *key, int dir);
aes128_ctx *aes128_new(const u8 *key, int dir);

void aes128_permute(const aes128_ctx *ctx, const u8 *in, u8 *out);

void aes128_encrypt(const u8 *key, const u8 *p, u8 *c);
void aes128_decrypt(const u8 *key, const u8 *c, u8 *p);

#endif
