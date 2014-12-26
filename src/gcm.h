#ifndef REBELTLS_GCM_H
#define REBELTLS_GCM_H

#include "bc.h"
#include "inttypes.h"

enum {
  GCM_ENCRYPT,
  GCM_DECRYPT
};

typedef struct {
  const bc_defn *defn;
  bc_ctx cctx;
  uint8 h[16];
  uint8 iv[12];
  uint32 ctr;
  uint adlen;
  uint mlen;
  int dir;
} gcm_ctx;

extern void gcm_init(gcm_ctx *ctx, const bc_defn *defn,
                     const uint8 *key, const uint8 *iv, int dir);
extern gcm_ctx *gcm_new(const bc_defn *defn, const uint8 *key,
                        const uint8 *iv, int dir);

extern void gcm_adupdate(gcm_ctx *ctx, const uint8 *ad, uint adlen);
extern void gcm_adfinal(gcm_ctx *ctx);

extern void gcm_update(gcm_ctx *ctx, const uint8 *in, uint8 *out, uint len);
extern void gcm_final(gcm_ctx *ctx, uint8 *tag);

extern void gcm_encrypt(const bc_defn *defn, const uint8 *key,
                        const uint8 *ad, uint adlen,
                        const uint8 *p, uint plen,
                        uint8 *c, uint8 *tag);

extern void gcm_decrypt(const bc_defn *defn, const uint8 *key,
                        const uint8 *ad, uint adlen,
                        const uint8 *c, uint clen,
                        uint8 *p, const uint8 *tag);

#endif
