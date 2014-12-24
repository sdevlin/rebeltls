#include <string.h>

#include "hash.h"
#include "hmac.h"
#include "log.h"
#include "prf.h"
#include "types.h"

static uint min(uint x, uint y)
{
  return x < y ? x : y;
}

extern void prf_expand(const hash_desc *desc,
                       const byte *secret, uint secretlen,
                       const byte *label, uint labellen,
                       const byte *seed, uint seedlen,
                       byte *out, uint outlen)
{
  hmac_ctx ctx;
  uint take;
  uint Alen;
  byte A[HASH_MAX_HASHLEN];
  byte h[HASH_MAX_HASHLEN];

  log_assert(labellen + seedlen <= sizeof A,
             "labellen + seedlen > sizeof A (%u > %lu)", labellen + seedlen, sizeof A);

  memcpy(A, label, labellen);
  memcpy(A + labellen, seed, seedlen);
  Alen = labellen + seedlen;

  while (outlen > 0) {
    hmac_digest(desc, secret, secretlen, A, Alen, A);
    Alen = desc->hashlen;

    hmac_init(&ctx, desc, secret, secretlen);
    hmac_update(&ctx, A, Alen);
    hmac_update(&ctx, label, labellen);
    hmac_update(&ctx, seed, seedlen);
    hmac_final(&ctx, h);

    take = min(outlen, desc->hashlen);
    memcpy(out, h, take);
    out += take;
    outlen -= take;
  }
}
