#include <gmp.h>

#include "rsa.h"


void rsa_init(rsa_key *key)
{
  mpz_init(key->e);
}
