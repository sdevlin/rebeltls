#include <stdlib.h>

#include "hash.h"

#define DEFINE_HASH(name, hlen, blen)                       \
  static void name##_hash_init(hash_ctx *ctx)               \
  {                                                         \
    name##_init((name##_ctx *)ctx);                         \
  }                                                         \
                                                            \
  static hash_ctx *name##_hash_new(void)                    \
  {                                                         \
    hash_ctx *ctx;                                          \
    ctx = malloc(sizeof *ctx);                              \
    name##_init((name##_ctx *)ctx);                         \
    return ctx;                                             \
  }                                                         \
                                                            \
  static void name##_hash_update(hash_ctx *ctx,             \
                                 const byte *m, uint mlen)  \
  {                                                         \
    name##_update((name##_ctx *)ctx, m, mlen);              \
  }                                                         \
                                                            \
  static void name##_hash_final(hash_ctx *ctx, byte *h)     \
  {                                                         \
    name##_final((name##_ctx *)ctx, h);                     \
  }                                                         \
                                                            \
  const hash_defn name##_defn = {                           \
    .hashlen = hlen,                                        \
    .blocklen = blen,                                       \
    .init = &name##_hash_init,                              \
    .new = &name##_hash_new,                                \
    .update = &name##_hash_update,                          \
    .final = &name##_hash_final,                            \
    .digest = &name##_digest                                \
  }

/* eliding trailing semicolon so it will be required on macro invocation */

#include "sha1.h"
DEFINE_HASH(sha1, 20, 64);

#include "sha224.h"
DEFINE_HASH(sha224, 28, 64);

#include "sha256.h"
DEFINE_HASH(sha256, 32, 64);

#include "sha384.h"
DEFINE_HASH(sha384, 48, 64);

#include "sha512.h"
DEFINE_HASH(sha512, 64, 128);

const hash_defn *hash_defns[] = {
  &sha1_defn,
  &sha224_defn,
  &sha256_defn,
  &sha384_defn,
  &sha512_defn
};
