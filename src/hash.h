#ifndef TLS_HASH_H
#define TLS_HASH_H

#include "sha512.h"
#include "types.h"

typedef sha512_ctx hash_ctx;

/* sha512 outputs its full internal state h */
#define HASH_MAX_HASHLEN (sizeof ((hash_ctx *)NULL)->h)
#define HASH_MAX_BLOCKLEN (sizeof ((hash_ctx *)NULL)->buf)

typedef struct {
  uint hashlen;
  uint blocklen;
  void (*init)(hash_ctx *ctx);
  hash_ctx *(*new)(void);
  void (*update)(hash_ctx *ctx, const byte *m, uint mlen);
  void (*final)(hash_ctx *ctx, byte *h);
  void (*digest)(const byte *m, uint mlen, byte *h);
} hash_defn;

enum {
  HASH_DEFN_SHA1,
  HASH_DEFN_SHA256,
  HASH_DEFN_SHA512
};

extern const hash_defn *hash_defns[];

#endif
