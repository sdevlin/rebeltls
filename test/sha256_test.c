#include <string.h>

#include "sha256.h"
#include "types.h"

byte client_hello_bin[] = {
  0x16, 0x03, 0x01, 0x00, 0xb6, 0x01, 0x00, 0x00, 0xb2, 0x03, 0x03, 0x53,
  0xe7, 0x89, 0xa7, 0x41, 0x0b, 0xad, 0xe1, 0x85, 0x26, 0xca, 0x31, 0x1b,
  0xce, 0x89, 0xfb, 0xe3, 0x94, 0x4e, 0x4d, 0x55, 0xfa, 0xc9, 0xdc, 0xa3,
  0xd2, 0x92, 0x41, 0x2b, 0x09, 0x7c, 0x62, 0x00, 0x00, 0x4a, 0x00, 0xff,
  0xc0, 0x24, 0xc0, 0x23, 0xc0, 0x0a, 0xc0, 0x09, 0xc0, 0x07, 0xc0, 0x08,
  0xc0, 0x28, 0xc0, 0x27, 0xc0, 0x14, 0xc0, 0x13, 0xc0, 0x11, 0xc0, 0x12,
  0xc0, 0x26, 0xc0, 0x25, 0xc0, 0x2a, 0xc0, 0x29, 0xc0, 0x05, 0xc0, 0x04,
  0xc0, 0x02, 0xc0, 0x03, 0xc0, 0x0f, 0xc0, 0x0e, 0xc0, 0x0c, 0xc0, 0x0d,
  0x00, 0x3d, 0x00, 0x3c, 0x00, 0x2f, 0x00, 0x05, 0x00, 0x04, 0x00, 0x35,
  0x00, 0x0a, 0x00, 0x67, 0x00, 0x6b, 0x00, 0x33, 0x00, 0x39, 0x00, 0x16,
  0x01, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x19, 0x00, 0x17, 0x00, 0x00,
  0x14, 0x6e, 0x65, 0x77, 0x73, 0x2e, 0x79, 0x63, 0x6f, 0x6d, 0x62, 0x69,
  0x6e, 0x61, 0x74, 0x6f, 0x72, 0x2e, 0x63, 0x6f, 0x6d, 0x00, 0x0a, 0x00,
  0x08, 0x00, 0x06, 0x00, 0x17, 0x00, 0x18, 0x00, 0x19, 0x00, 0x0b, 0x00,
  0x02, 0x01, 0x00, 0x00, 0x0d, 0x00, 0x0c, 0x00, 0x0a, 0x05, 0x01, 0x04,
  0x01, 0x02, 0x01, 0x04, 0x03, 0x02, 0x03
};

byte client_hello_bin_hash[] = {
  0xfa, 0xe1, 0x81, 0x1c, 0x5c, 0x30, 0x6d, 0xa2,
  0x50, 0x87, 0xb5, 0xa1, 0x90, 0x44, 0x36, 0x60,
  0xb9, 0xcc, 0xa5, 0xcf, 0x61, 0xbf, 0xe3, 0x13,
  0xde, 0x33, 0xe5, 0xe1, 0x73, 0x7b, 0xed, 0xce
};

int main(void)
{
  byte h[32];
  sha256_digest(client_hello_bin, sizeof client_hello_bin, h);
  return memcmp(h, client_hello_bin_hash, sizeof h);
}
