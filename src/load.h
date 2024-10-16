#ifndef MRBC_SRC_LOAD_H_
#define MRBC_SRC_LOAD_H_

#include "vm.h"

int mrbc_load_mrb(struct VM *vm, const void *bytecode);
int mrbc_load_irep(struct VM *vm, const void *bytecode);
//mrbc_value mrbc_irep_pool_value(struct VM *vm, int n);

#endif
