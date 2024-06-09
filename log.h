#ifndef LOG_H
#define LOG_H

#include "stdio.h"

#define LOG(message, ...)                                                      \
  fprintf(stderr, "%s:%i: " message "\n", __FILE__, __LINE__, ##__VA_ARGS__);

#endif
