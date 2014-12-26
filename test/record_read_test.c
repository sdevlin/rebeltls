#include <fcntl.h>
#include <string.h>

#include "io.h"
#include "record.h"

uint8 data[] = {
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
  0x01, 0x02, 0x01, 0x04, 0x03, 0x02, 0x03, 0x16, 0x03, 0x03, 0x00, 0x46,
  0x10, 0x00, 0x00, 0x42, 0x41, 0x04, 0xc8, 0xd1, 0xd1, 0xe4, 0x58, 0x72,
  0x2f, 0x4d, 0x58, 0x48, 0xcc, 0x0c, 0xca, 0xf2, 0xc6, 0x76, 0xb3, 0xfa,
  0x0e, 0x4a, 0x6b, 0x9d, 0xc9, 0x60, 0x0f, 0xcc, 0xca, 0xcd, 0x37, 0x34,
  0x25, 0x44, 0xf0, 0xfd, 0xa2, 0xdc, 0x6b, 0x4b, 0xbb, 0x48, 0x45, 0x44,
  0x1c, 0xb7, 0x2e, 0xe7, 0xc9, 0x37, 0xce, 0x4d, 0x4c, 0x4e, 0x78, 0x42,
  0x21, 0x4e, 0x35, 0x6f, 0x06, 0xbb, 0x69, 0x16, 0x05, 0x87
};

int main(void)
{
  struct record rec;
  int fd;
  io io;

  fd = open("client-hello.bin", O_RDONLY);
  io_fileinit(&io, fd);

  record_read(&io, &rec);
  record_read(&io, &rec);

  return 0;
}
