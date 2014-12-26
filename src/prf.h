#ifndef REBELTLS_PRF_H
#define REBELTLS_PRF_H

#include "hash.h"
#include "inttypes.h"

extern void prf_expand(const hash_desc *desc,
                       const u8 *secret, uint secretlen,
                       const u8 *label, uint labellen,
                       const u8 *seed, uint seedlen,
                       u8 *out, uint outlen);

#endif
