#ifndef MRBC_SRC_VM_H_
#define MRBC_SRC_VM_H_

#include "shims.h"
#include "value.h"

typedef struct VM {
  uint8_t *inst;
} mrbc_vm;

void mrbc_vm_run(mrbc_vm *vm, mrbc_value *regs);

#endif

