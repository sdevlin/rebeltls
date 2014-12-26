#include <stdlib.h>

#include "bc.h"

/* TODO this needs rethinking */

#define DEFINE_BC(name, klen, blen)                         \
  static void name##_bc_init(bc_ctx *ctx, const uint8 *key) \
  {                                                         \
    name##_init((name##_ctx *)ctx, key);                    \
  }                                                         \
                                                            \
  static bc_ctx *name##_bc_new(const uint8 *key)            \
  {                                                         \
    bc_ctx *ctx;                                            \
    ctx = malloc(sizeof *ctx);                              \
    name##_init((name##_ctx *)ctx, key);                    \
    return ctx;                                             \
  }                                                         \
                                                            \
  static void name##_bc_encrypt(const bc_ctx *ctx,          \
                                const uint8 *p, uint8 *c)   \
  {                                                         \
    name##_encrypt((const name##_ctx *)ctx, p, c);          \
  }                                                         \
                                                            \
  static void name##_bc_decrypt(const bc_ctx *ctx,          \
                                const uint8 *c, uint8 *p)   \
  {                                                         \
    name##_decrypt((const name##_ctx *)ctx, c, p);          \
  }                                                         \
                                                            \
  const bc_defn name##_defn = {                             \
    .keylen = klen,                                         \
    .blocklen = blen,                                       \
    .init = &name##_bc_init,                                \
    .new = &name##_bc_new,                                  \
    .encrypt = &name##_bc_encrypt,                          \
    .decrypt = &name##_bc_decrypt                           \
  }

/* eliding trailing semicolon so it will be required on macro invocation */

/* #include "aes128.h" */
/* DEFINE_BC(aes128, 16, 16); */

const bc_defn *bc_defns[] = {
  /* &aes128_defn */
  NULL
};
