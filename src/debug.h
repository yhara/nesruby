#include <stdlib.h>
#include "neslib.h"

static void put_str(unsigned int adr,const char *str)
{
  ppu_off();
  vram_adr(adr);
  while(1)
  {
    if(!*str) break;
    vram_put((*str++)-0x20);//-0x20 because ASCII code 0x20 is placed in tile 0 of the CHR
  }
  ppu_on_all();
}

static void put_digit(unsigned int adr, unsigned char i)
{
  char buf[10];
  itoa(i,buf,10);
  put_str(adr,buf);
}

static void panic(const char *str)
{
  put_str(NTADR_A(0, 0), str);
  while(1);
}
