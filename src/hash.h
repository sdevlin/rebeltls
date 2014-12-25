#ifndef REBELTLS_HASH_H
#define REBELTLS_HASH_H

#include "sha512.h"
#include "inttypes.h"

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
} hash_desc;

extern const hash_desc md5_desc;
extern const hash_desc sha1_desc;
extern const hash_desc sha224_desc;
extern const hash_desc sha256_desc;
extern const hash_desc sha384_desc;
extern const hash_desc sha512_desc;

enum {
  HASH_DESC_MD5,
  HASH_DESC_SHA1,
  HASH_DESC_SHA224,
  HASH_DESC_SHA256,
  HASH_DESC_SHA384,
  HASH_DESC_SHA512
};

extern const hash_desc *hash_descs[];

#endif
