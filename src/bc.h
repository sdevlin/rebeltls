#ifndef REBELTLS_BC_H
#define REBELTLS_BC_H

#include "aes128.h"
#include "inttypes.h"

/* TODO this needs rethinking */

typedef aes128_ctx bc_ctx;

#define BC_MAX_KEYLEN 16
#define BC_MAX_BLOCKLEN 16

typedef struct {
  uint keylen;
  uint blocklen;
  void (*init)(bc_ctx *ctx, const u8 *key);
  bc_ctx *(*new)(const u8 *key);
  void (*encrypt)(const bc_ctx *ctx, const u8 *p, u8 *c);
  void (*decrypt)(const bc_ctx *ctx, const u8 *c, u8 *p);
} bc_defn;

/* extern const bc_defn aes128_defn; */

/* enum { */
/*   BC_DEFN_AES128 */
/* }; */

extern const bc_defn *bc_defns[];

#endif
