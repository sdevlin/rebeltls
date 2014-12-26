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
  u8 h[16];
  u8 iv[12];
  u32 ctr;
  uint adlen;
  uint mlen;
  int dir;
} gcm_ctx;

extern void gcm_init(gcm_ctx *ctx, const bc_defn *defn,
                     const u8 *key, const u8 *iv, int dir);
extern gcm_ctx *gcm_new(const bc_defn *defn, const u8 *key,
                        const u8 *iv, int dir);

extern void gcm_adupdate(gcm_ctx *ctx, const u8 *ad, uint adlen);
extern void gcm_adfinal(gcm_ctx *ctx);

extern void gcm_update(gcm_ctx *ctx, const u8 *in, u8 *out, uint len);
extern void gcm_final(gcm_ctx *ctx, u8 *tag);

extern void gcm_encrypt(const bc_defn *defn, const u8 *key,
                        const u8 *ad, uint adlen,
                        const u8 *p, uint plen,
                        u8 *c, u8 *tag);

extern void gcm_decrypt(const bc_defn *defn, const u8 *key,
                        const u8 *ad, uint adlen,
                        const u8 *c, uint clen,
                        u8 *p, const u8 *tag);

#endif
