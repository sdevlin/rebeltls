#ifndef REBELTLS_PRF_H
#define REBELTLS_PRF_H

#include "hash.h"
#include "inttypes.h"

extern void prf_expand(const hash_desc *desc,
                       const uint8 *secret, uint secretlen,
                       const uint8 *label, uint labellen,
                       const uint8 *seed, uint seedlen,
                       uint8 *out, uint outlen);

#endif
