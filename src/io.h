#ifndef TLS_IO_H
#define TLS_IO_H

#include "types.h"

extern void io_read(int fd, byte *buf, uint buflen);
extern void io_write(int fd, const byte *buf, uint buflen);
extern void io_seek(int fd, uint len);

#endif
