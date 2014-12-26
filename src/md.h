#ifndef REBELTLS_MD_H
#define REBELTLS_MD_H

#include "inttypes.h"

typedef struct {
  uint mlen;
} md_ctx;

typedef struct {
  uint buflen;
  uint8 *(*buffer)(md_ctx *ctx);
  void (*compress)(md_ctx *ctx);
  uint mlenoffset;
  void (*packmlen)(md_ctx *ctx);
} md_defn;

extern void md_update(const md_defn *defn, md_ctx *ctx,
                      const uint8 *m, uint mlen);

extern void md_final(const md_defn *defn, md_ctx *ctx);

#endif
