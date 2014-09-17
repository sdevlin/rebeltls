#ifndef TLS_IO_H
#define TLS_IO_H

#include "types.h"

typedef union {
  int fd;
  struct {
    byte *buf;
    uint len;
    uint pos;
  } buf;
} io_src;

typedef struct {
  const struct io_defn *defn;
  io_src src;
} io;

void io_fileinit(io *io, int fd);
void io_netinit(io *io, int fd);
void io_bufinit(io *io, byte *buf, uint buflen);

int io_tryread(io *io, byte *buf, uint buflen);
int io_trywrite(io *io, const byte *buf, uint buflen);
int io_read(io *io, byte *buf, uint buflen);
int io_write(io *io, const byte *buf, uint buflen);

struct io_defn {
  int (*tryread)(io_src *src, byte *buf, uint buflen);
  int (*trywrite)(io_src *src, const byte *buf, uint buflen);
  int (*read)(io_src *src, byte *buf, uint buflen);
  int (*write)(io_src *src, const byte *buf, uint buflen);
};

extern const struct io_defn fileio_defn;
extern const struct io_defn netio_defn;
extern const struct io_defn bufio_defn;

#endif
