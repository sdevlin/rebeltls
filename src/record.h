#ifndef TLS_RECORD_H
#define TLS_RECORD_H

#include "types.h"

#define RECORD_MAX_LEN 18432

struct record {
  uint8 content_type;
  struct {
    uint8 major;
    uint8 minor;
  } protocol_version;
  uint16 length;
  byte fragment[RECORD_MAX_LEN];
};

extern int record_read(int fd, struct record *rec);
extern void record_write(int fd, const struct record *rec);

#endif
