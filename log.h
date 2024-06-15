#ifndef LOG_H
#define LOG_H

#include "stdio.h"

#define LOG_ENABLED

#define LOG_ALWAYS(message, ...)                                               \
  fprintf(stderr, "%s:%i: " message "\n", __FILE__, __LINE__, ##__VA_ARGS__);

#ifdef LOG_ENABLED
#define LOG(message, ...) LOG_ALWAYS(message, ##__VA_ARGS__)
#else
#define LOG(message, ...)
#endif

#endif
