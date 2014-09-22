#ifndef REBELTLS_BC_H
#define REBELTLS_BC_H

#include "aes128.h"
#include "types.h"

typedef aes128_ctx bc_ctx;

#define BC_MAX_KEYLEN 16
#define BC_MAX_BLOCKLEN 16

typedef struct {
  uint keylen;
  uint blocklen;
  void (*init)(bc_ctx *ctx, const byte *key);
  bc_ctx *(*new)(const byte *key);
  void (*encrypt)(const bc_ctx *ctx, const byte *p, byte *c);
  void (*decrypt)(const bc_ctx *ctx, const byte *c, byte *p);
} bc_defn;

extern const bc_defn aes128_defn;

enum {
  BC_DEFN_AES
};

extern const bc_defn *bc_defns[];

#endif
