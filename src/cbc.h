#ifndef REBELTLS_CBC_H
#define REBELTLS_CBC_H

#include "inttypes.h"
#include "bc.h"

enum {
  CBC_ENCRYPT,
  CBC_DECRYPT
};

typedef struct {
  bc_defn *cdefn;
  bc_ctx cctx;
  int dir;
} cbc_ctx;

extern void cbc_init(cbc_ctx *ctx, const struct bc_defn *cdefn,
                     const u8 *key, const u8 *iv, int dir);
extern cbc_ctx *cbc_new(const struct bc_defn *cdefn,
                        const u8 *key, const u8 *iv, int dir);

extern void cbc_update(cbc_ctx *ctx);

#endif
