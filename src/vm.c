#include <conio.h>
#include <string.h>
#include "opcode.h"
#include "value.h"
#include "vm.h"

static void op_loadi(mrbc_vm *vm, mrbc_value *regs)
{
  FETCH_BB();
  regs[a] = b;
}
static void op_loadi_0(mrbc_vm *vm, mrbc_value *regs)
{
  FETCH_B();
  regs[a] = 0;
}
static void op_loadi_1(mrbc_vm *vm, mrbc_value *regs)
{
  FETCH_B();
  regs[a] = 1;
}
static void op_loadi_2(mrbc_vm *vm, mrbc_value *regs)
{
  FETCH_B();
  regs[a] = 2;
}
static void op_loadi_3(mrbc_vm *vm, mrbc_value *regs)
{
  FETCH_B();
  regs[a] = 3;
}
static void op_loadi_4(mrbc_vm *vm, mrbc_value *regs)
{
  FETCH_B();
  regs[a] = 4;
}
static void op_loadi_5(mrbc_vm *vm, mrbc_value *regs)
{
  FETCH_B();
  regs[a] = 5;
}
static void op_loadi_6(mrbc_vm *vm, mrbc_value *regs)
{
  FETCH_B();
  regs[a] = 6;
}
static void op_loadi_7(mrbc_vm *vm, mrbc_value *regs)
{
  FETCH_B();
  regs[a] = 7;
}
static void op_return(mrbc_vm *vm, mrbc_value *regs)
{
  FETCH_B();
  cprintf("answer is %d", regs[a]);
}

static void op_stop(mrbc_vm *vm, mrbc_value *regs)
{
  //cprintf("stop");
}

void mrbc_vm_run(struct VM *vm, mrbc_value *regs)
{
  uint8_t op;
  while (1) {
    op = *vm->inst++;
    //cprintf("[%d]", op);
    switch (op) {
      case OP_LOADI: op_loadi(vm, regs); break;
      case OP_LOADI_0: op_loadi_0(vm, regs); break;
      case OP_LOADI_1: op_loadi_1(vm, regs); break;
      case OP_LOADI_2: op_loadi_2(vm, regs); break;
      case OP_LOADI_3: op_loadi_3(vm, regs); break;
      case OP_LOADI_4: op_loadi_4(vm, regs); break;
      case OP_LOADI_5: op_loadi_5(vm, regs); break;
      case OP_LOADI_6: op_loadi_6(vm, regs); break;
      case OP_LOADI_7: op_loadi_7(vm, regs); break;
      case OP_RETURN: op_return(vm, regs); break;
      case OP_STOP: op_stop(vm, regs); break;
      default:
        cprintf("Unknown opcode: %d", op);
        break;
    }
    if (op == OP_STOP) break;
  }
}