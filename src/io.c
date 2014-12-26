#include "io.h"

void io_fileinit(io *io, int fd)
{
  io->defn = &fileio_defn;
  io->src.fd = fd;
}

void io_netinit(io *io, int fd)
{
  io->defn = &netio_defn;
  io->src.fd = fd;
}

void io_bufinit(io *io, u8 *buf, uint buflen)
{
  io->defn = &netio_defn;
  io->src.buf.buf = buf;
  io->src.buf.len = buflen;
  io->src.buf.pos = 0;
}

int io_tryread(io *io, u8 *buf, uint buflen)
{
  return io->defn->tryread(&io->src, buf, buflen);
}

int io_trywrite(io *io, const u8 *buf, uint buflen)
{
  return io->defn->trywrite(&io->src, buf, buflen);
}

int io_read(io *io, u8 *buf, uint buflen)
{
  return io->defn->read(&io->src, buf, buflen);
}

int io_write(io *io, const u8 *buf, uint buflen)
{
  return io->defn->write(&io->src, buf, buflen);
}
