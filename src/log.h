#ifndef TLS_LOG_H
#define TLS_LOG_H

#include <stdio.h>
#include <stdlib.h>

#define log_info(fmt, ...)                                              \
  fprintf(stderr, "INFO %s:%d (%s) " fmt "\n",                          \
          __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define log_warn(fmt, ...)                                              \
  fprintf(stderr, "WARN %s:%d (%s) " fmt "\n",                          \
          __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define log_error(fmt, ...)                                             \
  fprintf(stderr, "ERROR %s:%d (%s) " fmt "\n",                         \
          __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define log_assert(expr, fmt, ...)              \
  do {                                          \
    if (!(expr)) {                              \
      log_error(fmt, ##__VA_ARGS__);            \
      exit(1);                                  \
    }                                           \
  } while (0)

#define log_abort(fmt, ...) log_assert(0, fmt, ##__VA_ARGS__)

#endif