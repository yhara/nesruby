#include "opcode.h"
#include "_autogen_builtin_symbol.h"
#include "value.h"
#include "vm.h"
#include "debug.h"

static void op_loadi(mrbc_vm *vm, mrbc_value *regs)
{
  FETCH_BB();

  mrbc_decref(&regs[a]);
  mrbc_set_integer(&regs[a], b);
}
static void op_loadineg( mrbc_vm *vm, mrbc_value *regs)
{
  FETCH_BB();

  mrbc_decref(&regs[a]);
  mrbc_set_integer(&regs[a], -(mrbc_int_t)b);
}
static void op_loadi_n(mrbc_vm *vm, mrbc_value *regs)
{
  int opcode = vm->inst[-1];
  int n = opcode - OP_LOADI_0;

  FETCH_B();

  mrbc_decref(&regs[a]);
  mrbc_set_integer(&regs[a], n);
}

static void op_loadsym( mrbc_vm *vm, mrbc_value *regs )
{
  FETCH_BB();

  mrbc_decref(&regs[a]);
  mrbc_set_symbol(&regs[a], mrbc_irep_symbol_id(vm->cur_irep, b));
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
  (void)regs; // Surpress "never used" warning

  vm->inst += (int16_t)a;
}

static int line = 0;
static void op_ssend( mrbc_vm *vm, mrbc_value *regs )
{
  mrbc_sym sym;
  FETCH_BBB();

  mrbc_decref( &regs[a] );
  //TODO
  //regs[a] = *mrbc_get_self( vm, regs );
  mrbc_incref( &regs[a] );

  //TODO
  sym = mrbc_irep_symbol_id(vm->cur_irep, b);
  //send_by_name( vm, mrbc_irep_symbol_id(vm->cur_irep, b), a, c );
  if (sym == MRBC_SYM(hello)) {
    put_str(NTADR_A(0,line),"HELLO");
  } else if (sym == MRBC_SYM(bye)) {
    put_str(NTADR_A(0,line),"BYE");
  } 
  line++;
}

static void op_return(mrbc_vm *vm, mrbc_value *regs)
{
  FETCH_B();
  //put_digit(NTADR_A(0,0), regs[a].i);
}

static void op_stop(mrbc_vm *vm, mrbc_value *regs)
{
  (void)vm; // Surpress "never used" warning
  (void)regs; // Surpress "never used" warning
  //put_str(NTADR_A(0,0),"STOP");
}

void mrbc_vm_run(struct VM *vm)
{
  int i;
  uint8_t op;
  while (1) {
    mrbc_value *regs = vm->cur_regs;
    op = *vm->inst++;

    //put_digit(NTADR_A(0,0),op);
    //for(i=0;i<10000;i++);
    switch (op) {
      case OP_LOADI: op_loadi(vm, regs); break;
      case OP_LOADINEG:   op_loadineg   (vm, regs); break;
      case OP_LOADI__1:   // fall through
      case OP_LOADI_0:    // fall through
      case OP_LOADI_1:    // fall through
      case OP_LOADI_2:    // fall through
      case OP_LOADI_3:    // fall through
      case OP_LOADI_4:    // fall through
      case OP_LOADI_5:    // fall through
      case OP_LOADI_6:    // fall through
      case OP_LOADI_7: op_loadi_n(vm, regs); break;

      case OP_LOADSYM:    op_loadsym    (vm, regs); break;
      case OP_LOADNIL:    op_loadnil    (vm, regs); break;

      case OP_LOADT:      op_loadt      (vm, regs); break;
      case OP_LOADF:      op_loadf      (vm, regs); break;

      case OP_JMP:        op_jmp        (vm, regs); break;

      case OP_SSEND:      op_ssend      (vm, regs); break;

      case OP_RETURN: op_return(vm, regs); break;
      case OP_STOP: op_stop(vm, regs); break;
      default:
        //cprintf("[Unknown opcode: %d]", op);
        put_str(NTADR_A(0,0),"UNKNOWN OPCODE");
        break;
    }
    if (op == OP_STOP) break;
  }
}
