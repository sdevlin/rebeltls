#ifndef REBELTLS_RSA_H
#define REBELTLS_RSA_H

#include <gmp.h>

typedef struct {
  mpz_t e;
  mpz_t n;
} rsa_pubkey;

typedef struct {
  rsa_pubkey pubkey;
  mpz_t d;
  mpz_t p;
  mpz_t q;
  mpz_t dp;
  mpz_t dq;
  mpz_t qinv;
} rsa_keypair;

extern void rsa_init(rsa_keypair *key);

#endif
