#include <string.h>

#include "rc4.h"

int main(void)
{
  uint8 key[] = "Secret";
  uint8 p[] = "Attack at dawn";
  uint8 c[(sizeof p)-1];
  uint8 expected[] = {
    0x45, 0xa0, 0x1f, 0x64, 0x5f, 0xc3, 0x5b, 0x38,
    0x35, 0x52, 0x54, 0x4b, 0x9b, 0xf5
  };

  rc4_encrypt(key, (sizeof key)-1, p, (sizeof p)-1, c);

  return memcmp(c, expected, sizeof c);
}
