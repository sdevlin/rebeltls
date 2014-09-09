#ifndef TLS_RSA_H
#define TLS_RSA_H

#include <gmp.h>

typedef struct {
  mpz_t e;
} rsa_key;

extern void rsa_init(rsa_key *key);

#endif
