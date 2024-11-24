#include <stdlib.h>
#include "neslib.h"

static char put_str(unsigned int adr, const char *str)
{
  char i = 0;
  ppu_off();
  vram_adr(adr);
  while(1)
  {
    if(!*str) break;
    vram_put((*str++)-0x20);//-0x20 because ASCII code 0x20 is placed in tile 0 of the CHR
    i++;
  }
  ppu_on_all();
  return i;
}

static void put_digit(unsigned int adr, unsigned char digit)
{
  char buf[10];
  itoa(digit,buf,10);
  put_str(adr, buf);
}

static char col = 0;
static char line = 0;
static void d_puts(const char *str)
{
  put_str(NTADR_A(col, line), str);
  line++;
  col = 0;
}
static void d_print(const char *str)
{
  col += put_str(NTADR_A(col, line), str);
}
static void d_putd(signed int i)
{
  char buf[10];
  itoa(i,buf,10);
  d_print(buf);
}

static void panic(const char *str)
{
  put_str(NTADR_A(0, 0), str);
  while(1);
}
