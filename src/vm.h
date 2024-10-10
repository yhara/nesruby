#ifndef MRBC_SRC_VM_H_
#define MRBC_SRC_VM_H_

#include "shims.h"
#include "value.h"

typedef struct VM {
  uint8_t *inst;
} mrb_vm;

void mrb_vm_run(mrb_vm *vm, mrb_value *regs);

#endif

