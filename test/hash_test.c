#include "hash.h"
#include "sha256.h"

int main(void)
{
  const hash_defn *defn;
  int err;

  err = hash_define("sha256", &sha256_defn);
  if (err != 0) {
    return 1;
  }

  defn = hash_lookup("sha256");
  if (defn != &sha256_defn) {
    return 1;
  }

  return 0;
}
