#include <stdlib.h> // for itoa
#include "debug.h"

#ifdef __CC65__
  #include "neslib.h"

  char put_str(unsigned int adr, const char *str)
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

  void put_digit(unsigned int adr, signed int digit)
  {
    char buf[10];
    itoa(digit,buf,10);
    put_str(adr, buf);
  }

  void d_puts(const char *str)
  {
    put_str(NTADR_A(debug_col, debug_line), str);
    debug_line++;
    debug_col = 0;
  }
  void d_print(const char *str)
  {
    debug_col += put_str(NTADR_A(debug_col, debug_line), str);
  }
  void d_putd(signed int i)
  {
    char buf[10];
    itoa(i,buf,10);
    d_print(buf);
  }

  void panic(const char *str)
  {
    put_str(NTADR_A(0, 0), str);
    while(1);
  }

#else
  #include <stdio.h>

  void d_puts(const char *str) { puts(str); }
  void d_print(const char *str) { printf("%s", str); }
  void d_putd(signed int i){ printf("%d", i); }

  void panic(const char *str)
  {
    puts(str);
    exit(1);
  }

#endif
