#include <stdlib.h>

#include "hash.h"

#define DESCRIBE_HASH(name, hlen, blen)                     \
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
                                 const u8 *m, uint mlen) \
  {                                                         \
    name##_update((name##_ctx *)ctx, m, mlen);              \
  }                                                         \
                                                            \
  static void name##_hash_final(hash_ctx *ctx, u8 *h)    \
  {                                                         \
    name##_final((name##_ctx *)ctx, h);                     \
  }                                                         \
                                                            \
  const hash_desc name##_desc = {                           \
    .hashlen = hlen,                                        \
    .blocklen = blen,                                       \
    .init = &name##_hash_init,                              \
    .new = &name##_hash_new,                                \
    .update = &name##_hash_update,                          \
    .final = &name##_hash_final,                            \
    .digest = &name##_digest                                \
  };

#include "md5.h"
DESCRIBE_HASH(md5, 16, 64)

#include "sha1.h"
DESCRIBE_HASH(sha1, 20, 64)

#include "sha224.h"
DESCRIBE_HASH(sha224, 28, 64)

#include "sha256.h"
DESCRIBE_HASH(sha256, 32, 64)

#include "sha384.h"
DESCRIBE_HASH(sha384, 48, 64)

#include "sha512.h"
DESCRIBE_HASH(sha512, 64, 128)

const hash_desc *hash_descs[] = {
  &md5_desc,
  &sha1_desc,
  &sha224_desc,
  &sha256_desc,
  &sha384_desc,
  &sha512_desc
};
