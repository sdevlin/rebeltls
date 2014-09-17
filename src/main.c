#include <string.h>
#include <stdio.h>
#include <fcntl.h>
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

/* static int setup(void); */

static struct record rec;

#include <time.h>

int main(void)
{
  int i;
  int fd;
  io io;

  fd = open("samples/server-all.bin", O_RDONLY);
  io_fileinit(&io, fd);

  for (i = 0; i < 10; i += 1) {
    record_read(&io, &rec);
    printf("%d\n", rec.content_type);
  }

  close(fd);

  return 0;
}

/* static int setup(void) */
/* { */
/*   struct addrinfo hints; */
/*   struct addrinfo *info; */
/*   int sockfd; */

/*   memset(&hints, 0, sizeof hints); */
/*   hints.ai_family = AF_UNSPEC; */
/*   hints.ai_socktype = SOCK_STREAM; */
/*   hints.ai_flags = AI_PASSIVE; */

/*   getaddrinfo("google.com", "443", &hints, &info); */
/*   sockfd = socket(info->ai_family, info->ai_socktype, info->ai_protocol); */
/*   connect(sockfd, info->ai_addr, info->ai_addrlen); */

/*   freeaddrinfo(info); */
/*   return sockfd; */
/* } */
