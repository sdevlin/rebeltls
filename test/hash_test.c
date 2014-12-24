#include "hash.h"

int main(void)
{
  if (&sha1_desc != hash_descs[HASH_DESC_SHA1]) {
    return 1;
  }

  if (&sha256_desc != hash_descs[HASH_DESC_SHA256]) {
    return 1;
  }

  if (&sha512_desc != hash_descs[HASH_DESC_SHA512]) {
    return 1;
  }

  return 0;
}
