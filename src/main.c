#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "record.h"
#include "types.h"
#include "random.h"

static int setup(void);

static struct record rec;

#include <time.h>

int main(void)
{
  int fd;

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
    uint16 session_id;
  } client_hello;

  client_hello.msg_type = 1;

  /* serialize as 24-bit */
  client_hello.length = sizeof client_hello;

  client_hello.protocol_version.major = 3;
  client_hello.protocol_version.minor = 1;

  client_hello.random.gmt_unix_time = time(NULL);
  random_bytes(client_hello.random.random_bytes,
               sizeof client_hello.random.random_bytes);

  client_hello.session_id = 0;


  rec.content_type = 22;
  rec.protocol_version.major = 3;
  rec.protocol_version.minor = 1;
  /* rec.length = (sizeof payload) - 1; */
  /* memcpy(rec.fragment, payload, rec.length); */

  fd = setup();

  record_write(fd, &rec);

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
