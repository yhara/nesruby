#ifndef MRBC_SRC_SHIMS_H_
#define MRBC_SRC_SHIMS_H_

#ifdef __CC65__
  typedef char int8_t;
  typedef unsigned char uint8_t;
  typedef int int16_t;
  typedef unsigned int uint16_t;
#else
  #include <stdint.h>
#endif

#endif
