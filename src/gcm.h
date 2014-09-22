#ifndef REBELTLS_GCM_H
#define REBELTLS_GCM_H

#include "bc.h"
#include "types.h"

enum {
  GCM_ENCRYPT,
  GCM_DECRYPT
};

typedef struct {
  const bc_defn *defn;
  bc_ctx cctx;
  byte h[16];
  byte iv[12];
  uint32 ctr;
  uint adlen;
  uint mlen;
  int dir;
} gcm_ctx;

extern void gcm_init(gcm_ctx *ctx, const bc_defn *defn,
                     const byte *key, const byte *iv, int dir);
extern gcm_ctx *gcm_new(const bc_defn *defn, const byte *key,
                        const byte *iv, int dir);

extern void gcm_adupdate(gcm_ctx *ctx, const byte *ad, uint adlen);
extern void gcm_adfinal(gcm_ctx *ctx);

extern void gcm_update(gcm_ctx *ctx, const byte *in, byte *out, uint len);
extern void gcm_final(gcm_ctx *ctx, byte *tag);

extern void gcm_encrypt(const bc_defn *defn, const byte *key,
                        const byte *ad, uint adlen,
                        const byte *p, uint plen,
                        byte *c, byte *tag);

extern void gcm_decrypt(const bc_defn *defn, const byte *key,
                        const byte *ad, uint adlen,
                        const byte *c, uint clen,
                        byte *p, const byte *tag);

#endif
