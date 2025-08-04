//#include "neslib.h"
//#include "src/debug.h"
#include "src/load.h"
//#include "src/opcode.h"
#include "src/shims.h"
//#include "src/value.h"
#include "src/vm.h"
#include "src/mrb_data.h"

static uint8_t memory_pool[MRBC_MEMORY_SIZE];
static mrbc_vm vm;
static mrbc_value mrbc_regs[MAX_REGS];
void run_ruby()
{
  uint8_t *p = (uint8_t *)mrb_data;
  vm.cur_regs = mrbc_regs;
  mrbc_load_mrb(&vm, mrb_data);
}

int main(void)
{
  run_ruby();
  return 0;
}
