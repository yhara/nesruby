#include <conio.h>
#include <string.h>
#include "mrb_data.h"

#include "opcode.h"
#include "shims.h"
#include "value.h"
#include "vm.h"
// cc65 memo
// - no "inline" keyword
// limitation
// - ilen must be 16bit

static uint16_t bin_to_uint16( const void *s )
{
  uint16_t x = *((uint16_t *)s);
  x = (x << 8) | (x >> 8);
  return x;
}
//static uint32_t bin_to_uint32( const void *s )
//{
//  uint32_t x = *((uint32_t *)s);
//  x = (x << 24) | ((x & 0xff00) << 8) | ((x >> 8) & 0xff00) | (x >> 24);
//  return x;
//}

static mrb_vm vm;


void run_ruby();
static const int SIZE_RITE_BINARY_HEADER = 20;
static const int SIZE_RITE_SECTION_HEADER = 12;
static const char IREP[4] = "IREP";

const char message[] = "Hello cc65!";

// vm
static const uint8_t MAX_REGS_SIZE = 16;
static mrb_value mrb_regs[16];

int main(void)
{
    unsigned int frame = 0;

    clrscr();
    //gotoxy((32-strlen(message))/2, 30/2);
    gotoxy(0, 15);
    run_ruby();
    //cprintf("value: %03d", mrb_data[0]);
    while(1) {
        gotoxy(0, 0);
        cprintf("%8d", frame++);
    }

    return 0;
}

static char buf[10];
void run_ruby()
{
  uint8_t *p = (uint8_t *)mrb_data;
  uint16_t ilen;
  uint16_t clen;
  // Load .mrb
  p += SIZE_RITE_BINARY_HEADER;
  //cprintf("%3s", (char *)mrb_data);
  // Load IREP section
  p += SIZE_RITE_SECTION_HEADER;
  // Load an IREP
  p += 4; // skip record size
  p += 2; // nlocals
  p += 2; // nregs
  p += 2; // rlen
  clen = bin_to_uint16(p); p += 2;
  p += 2; ilen = bin_to_uint16(p); p += 2;
  // Load an ISEQ
  vm.inst = p;
  mrb_vm_run(&vm, mrb_regs);
}
