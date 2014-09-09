#include <stdlib.h>

#include <gmp.h>

#include "rsa.h"


void rsa_init(rsa_keypair *keypair)
{
  mpz_init(keypair->pubkey.e);
}

rsa_keypair *rsa_new(void)
{
  rsa_keypair *keypair;
  keypair = malloc(sizeof *keypair);
  rsa_init(keypair);
  return keypair;
}
