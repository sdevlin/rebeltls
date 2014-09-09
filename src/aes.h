#ifndef TLS_AES_H
#define TLS_AES_H

#include "types.h"

#define Nk 4
#define Nb 4
#define Nr 10

#define KEYSIZE (4*Nk)
#define BLKSIZE (4*Nb)

typedef struct {
  uint32 w[44];
} aes_ctx;

void aes_init(aes_ctx *ctx, byte *key);
void aes_encrypt(aes_ctx *ctx, byte *p, byte *c);

#endif
