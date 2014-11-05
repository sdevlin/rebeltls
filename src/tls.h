#ifndef REBELTLS_TLS_H
#define REBELTLS_TLS_H

#include "types.h"

enum {
  CE_SERVER,
  CE_CLIENT
};

enum {
  PRFA_TLS_PRF_SHA256
};

enum bulk_cipher_algorithm {
  BCA_NULL,
  BCA_RC4,
  BCA_3DES,
  BCA_AES
};

enum {
  CT_STREAM,
  CT_BLOCK,
  CT_AEAD
};

enum {
  MAC_NULL,
  MAC_HMAC_MD5,
  MAC_HMAC_SHA1,
  MAC_HMAC_SHA256,
  MAC_HMAC_SHA384,
  MAC_HMAC_SHA512
};

typedef struct {
  uint8 conn_end;
} tls_ctx;

#endif
