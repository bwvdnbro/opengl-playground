#ifndef ERROR_H
#define ERROR_H

#include "stdio.h"

#define ASSERT(condition, message, ...)                                        \
  if (!(condition)) {                                                          \
    fprintf(stderr, "%s:%i Failed assertion: \"%s\" (" message ")\n",          \
            __FILE__, __LINE__, #condition, ##__VA_ARGS__);                    \
    abort();                                                                   \
  }

#endif
