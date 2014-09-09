#include "hash.h"

int main(void)
{
  if (&sha1_defn != hash_defns[HASH_DEFN_SHA1]) {
    return 1;
  }

  if (&sha256_defn != hash_defns[HASH_DEFN_SHA256]) {
    return 1;
  }

  if (&sha512_defn != hash_defns[HASH_DEFN_SHA512]) {
    return 1;
  }

  return 0;
}
