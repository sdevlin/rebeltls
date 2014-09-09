#ifndef TLS_PRF_H
#define TLS_PRF_H

#include "hash.h"
#include "types.h"

extern void prf_expand(const hash_defn *defn,
                       const byte *secret, uint secretlen,
                       const byte *label, uint labellen,
                       const byte *seed, uint seedlen,
                       byte *out, uint outlen);

#endif
