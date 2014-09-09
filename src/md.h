#ifndef TLS_MD_H
#define TLS_MD_H

#include "types.h"

typedef struct {
  uint mlen;
} md_ctx;

typedef struct {
  uint buflen;
  byte *(*buffer)(md_ctx *ctx);
  void (*compress)(md_ctx *ctx);
  uint mlenoffset;
  void (*packmlen)(md_ctx *ctx);
} md_defn;

extern void md_update(const md_defn *defn, md_ctx *ctx,
                      const byte *m, uint mlen);

extern void md_final(const md_defn *defn, md_ctx *ctx);

#endif
