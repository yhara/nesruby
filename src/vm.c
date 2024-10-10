#include <conio.h>
#include <string.h>
#include "opcode.h"
#include "value.h"
#include "vm.h"

static void op_loadi(mrbc_vm *vm, mrbc_value *regs)
{
  FETCH_BB();

  mrbc_decref(&regs[a]);
  mrbc_set_integer(&regs[a], b);
}
static void op_loadi_n(mrbc_vm *vm, mrbc_value *regs)
{
  int opcode = vm->inst[-1];
  int n = opcode - OP_LOADI_0;

  FETCH_B();

  mrbc_decref(&regs[a]);
  mrbc_set_integer(&regs[a], n);
}
static void op_loadnil( mrbc_vm *vm, mrbc_value *regs )
{
  FETCH_B();

  mrbc_decref(&regs[a]);
  mrbc_set_nil(&regs[a]);
}
static void op_loadt( mrbc_vm *vm, mrbc_value *regs )
{
  FETCH_B();

  mrbc_decref(&regs[a]);
  mrbc_set_true(&regs[a]);
}
static void op_loadf( mrbc_vm *vm, mrbc_value *regs )
{
  FETCH_B();

  mrbc_decref(&regs[a]);
  mrbc_set_false(&regs[a]);
}

static void op_jmp( mrbc_vm *vm, mrbc_value *regs)
{
  FETCH_S();

  vm->inst += (int16_t)a;
}

static void op_ssend( mrbc_vm *vm, mrbc_value *regs )
{
  FETCH_BBB();

  mrbc_decref( &regs[a] );
  //TODO
  //regs[a] = *mrbc_get_self( vm, regs );
  mrbc_incref( &regs[a] );

  //TODO
  //send_by_name( vm, mrbc_irep_symbol_id(vm->cur_irep, b), a, c );
  cprintf("hello");
}

static void op_return(mrbc_vm *vm, mrbc_value *regs)
{
  FETCH_B();
  cprintf("answer is %d", regs[a].i);
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
      case OP_LOADI__1:   // fall through
      case OP_LOADI_0:    // fall through
      case OP_LOADI_1:    // fall through
      case OP_LOADI_2:    // fall through
      case OP_LOADI_3:    // fall through
      case OP_LOADI_4:    // fall through
      case OP_LOADI_5:    // fall through
      case OP_LOADI_6:    // fall through
      case OP_LOADI_7: op_loadi_n(vm, regs); break;
      case OP_LOADNIL:    op_loadnil    (vm, regs); break;
      case OP_LOADT:      op_loadt      (vm, regs); break;
      case OP_LOADF:      op_loadf      (vm, regs); break;

      case OP_JMP:        op_jmp        (vm, regs); break;

      case OP_SSEND:      op_ssend      (vm, regs); break;

      case OP_RETURN: op_return(vm, regs); break;
      case OP_STOP: op_stop(vm, regs); break;
      default:
        cprintf("Unknown opcode: %d", op);
        break;
    }
    if (op == OP_STOP) break;
  }
}
