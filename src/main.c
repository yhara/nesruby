#include <conio.h>
#include <string.h>
#include "mrb_data.h"

#include "load.h"
#include "opcode.h"
#include "shims.h"
#include "value.h"
#include "vm.h"
// cc65 memo
// - no "inline" keyword
// limitation
// - ilen must be 16bit

static mrbc_vm vm;

void run_ruby();

const char message[] = "Hello cc65!";

// vm
static const uint8_t MAX_REGS_SIZE = 16;
static mrbc_value mrbc_regs[16];

int main(void)
{
    unsigned int frame = 0;

    clrscr();
    gotoxy(0, 0);
    run_ruby();

    //while(1) {
        //gotoxy(0, 0);
        //cprintf("%8d", frame++);
        //break;
    //}

    return 0;
}

void run_ruby()
{
  uint8_t *p = (uint8_t *)mrb_data;
//  uint16_t ilen;
//  uint16_t clen;
  // Load .mrb
  //p += SIZE_RITE_BINARY_HEADER;
  // Load IREP section
  //p += SIZE_RITE_SECTION_HEADER;
  // Load an IREP
//  vm.cur_irep = todo;
//  p += 4; // skip record size
//  p += 2; // nlocals
//  p += 2; // nregs
//  p += 2; // rlen
//  clen = bin_to_uint16(p); p += 2;
//  p += 2; ilen = bin_to_uint16(p); p += 2;
//  // Load an ISEQ
//  vm.inst = p;
//  vm.cur_regs = mrbc_regs;
  //mrbc_vm_run(&vm, mrb_data);
  mrbc_load_mrb(&vm, mrb_data);
}
