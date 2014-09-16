#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "bindata.h"
#include "io.h"
#include "record.h"
#include "types.h"
#include "random.h"
#include "vector.h"

static int setup(void);

static struct record rec;

#include <time.h>

int main(void)
{
  int fd;
  io io;

  struct {
    uint8 msg_type;
    uint32 length;
    struct {
      uint8 major;
      uint8 minor;
    } protocol_version;
    struct {
      uint32 gmt_unix_time;
      byte random_bytes[28];
    } random;
    byte session_id[23];
  } client_hello;

  struct vector sessid_vec;

  client_hello.msg_type = 1;

  /* serialize as 24-bit */
  client_hello.length = 0x38;

  client_hello.protocol_version.major = 3;
  client_hello.protocol_version.minor = 1;

  client_hello.random.gmt_unix_time = time(NULL);
  random_bytes(client_hello.random.random_bytes,
               sizeof client_hello.random.random_bytes);

  random_bytes(client_hello.session_id, sizeof client_hello.session_id);
  sessid_vec = vector_new(client_hello.session_id,
                          sizeof client_hello.session_id);

  rec.content_type = 22;
  rec.protocol_version.major = 3;
  rec.protocol_version.minor = 1;
  rec.length = bindata_pack(rec.fragment, "! B T BB LB[28] B<0..32>",
                            client_hello.msg_type,
                            client_hello.length,
                            client_hello.protocol_version.major,
                            client_hello.protocol_version.minor,
                            client_hello.random.gmt_unix_time,
                            client_hello.random.random_bytes,
                            &sessid_vec);

  fd = setup();
  io_fileinit(&io, STDOUT_FILENO);

  record_write(&io, &rec);

  close(fd);

  return 0;
}

static int setup(void)
{
  struct addrinfo hints;
  struct addrinfo *info;
  int sockfd;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  getaddrinfo("google.com", "443", &hints, &info);
  sockfd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
  connect(sockfd, info->ai_addr, info->ai_addrlen);

  freeaddrinfo(info);
  return sockfd;
}
