#ifndef MRBC_SRC_DEBUG_H_
#define MRBC_SRC_DEBUG_H_

char put_str(unsigned int adr, const char *str);
void put_digit(unsigned int adr, signed int digit);
static char debug_col = 0;
static char debug_line = 0;
void d_puts(const char *str);
void d_print(const char *str);
void d_putd(signed int i);
void panic(const char *str);

#endif
