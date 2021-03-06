#ifndef REBELTLS_MD5_H
#define REBELTLS_MD5_H

#include "inttypes.h"

typedef struct {
  uint mlen;
  u32 h[4];
  u8 buf[64];
} md5_ctx;

extern const u32 md5_initstate[4];

extern void md5_init(md5_ctx *ctx);
extern md5_ctx *md5_new(void);

extern void md5_reset(md5_ctx *ctx);

extern void md5_compress(md5_ctx *ctx);

extern void md5_update(md5_ctx *ctx, const u8 *m, uint mlen);
extern void md5_final(md5_ctx *ctx, u8 *h);

extern void md5_digest(const u8 *m, uint mlen, u8 *h);

#endif
