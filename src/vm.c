#include "opcode.h"
#include "_autogen_builtin_symbol.h"
#include "value.h"
#include "vm.h"
#include "debug.h"

static void op_nop( mrbc_vm *vm, mrbc_value *regs )
{
  FETCH_Z();
  (void)vm;   // Surpress "never used" warning
  (void)regs; // Surpress "never used" warning
}
static void op_move( mrbc_vm *vm, mrbc_value *regs)
{
  FETCH_BB();

  mrbc_incref(&regs[b]);
  mrbc_decref(&regs[a]);
  regs[a] = regs[b];
}
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

static void op_jmpnot( mrbc_vm *vm, mrbc_value *regs)
{
  FETCH_BS();

  if( regs[a].tt <= MRBC_TT_FALSE ) {
    vm->inst += (int16_t)b;
  }
}

const static char SPR_ARROW = 0;
const static char SPR_RUBY = 4;
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
  switch (sym) {
    case MRBC_SYM(wait_frame):
      ppu_wait_frame();
      break;
    case MRBC_SYM(draw_arrow):
      oam_spr(regs[a+1].i,
              regs[a+2].i, 0x45, 1, SPR_ARROW);
      break;
    case MRBC_SYM(draw_ruby):
      oam_spr(regs[a+1].i,
              regs[a+2].i, 0x46, 2, SPR_RUBY);
      break;
    case MRBC_SYM(play_sound):
      sfx_play(regs[a+1].i, regs[a+2].i);
      break;
    case MRBC_SYM(pad_trigger):
      mrbc_set_integer(&regs[a], pad_trigger(regs[a+1].i));
      break;
    case MRBC_SYM(btn_a_pressed):
      mrbc_set_bool(&regs[a], regs[a+1].i&PAD_A);
      break;
    case MRBC_SYM(show_title):
      put_str(NTADR_A(10, 10), "RUBY CATCHER");
      put_str(NTADR_A(11, 12), "PRESS BTN");
      break;
    case MRBC_SYM(hide_title):
      put_str(NTADR_A(10, 10), "            ");
      put_str(NTADR_A(11, 12), "         ");
      break;
    default:
      panic("UNKNOWN METHOD");
  }
}

static void op_return(mrbc_vm *vm, mrbc_value *regs)
{
  FETCH_B();
  (void)regs; // Surpress "never used" warning
  //put_digit(NTADR_A(0,0), regs[a].i);
}

static void op_add( mrbc_vm *vm, mrbc_value *regs)
{
  FETCH_B();

  if( regs[a].tt == MRBC_TT_INTEGER ) {
    if( regs[a+1].tt == MRBC_TT_INTEGER ) {     // in case of Integer, Integer
      regs[a].i += regs[a+1].i;
      return;
    }
  }

  // other case
  panic("TODO: GENERIC ADD");
  //send_by_name( vm, MRBC_SYM(PLUS), a, 1 );
}
static void op_addi( mrbc_vm *vm, mrbc_value *regs )
{
  FETCH_BB();

  if( regs[a].tt == MRBC_TT_INTEGER ) {
    regs[a].i += b;
    return;
  }

  panic("ADDI FAILED");
  //mrbc_raise(vm, MRBC_CLASS(TypeError), "no implicit conversion of Integer");
}
static void op_sub( mrbc_vm *vm, mrbc_value *regs )
{
  FETCH_B();

  if( regs[a].tt == MRBC_TT_INTEGER ) {
    if( regs[a+1].tt == MRBC_TT_INTEGER ) {     // in case of Integer, Integer
      regs[a].i -= regs[a+1].i;
      return;
    }
  }

  // other case
  panic("TODO: GENERIC SUB");
  //send_by_name( vm, MRBC_SYM(MINUS), a, 1 );
}
static void op_subi( mrbc_vm *vm, mrbc_value *regs )
{
  FETCH_BB();

  if( regs[a].tt == MRBC_TT_INTEGER ) {
    regs[a].i -= b;
    return;
  }

  panic("SUBI FAILED");
  //mrbc_raise(vm, MRBC_CLASS(TypeError), "no implicit conversion of Integer");
}

static void op_eq( mrbc_vm *vm, mrbc_value *regs)
{
  int result;
  FETCH_B();

//  if (regs[a].tt == MRBC_TT_OBJECT) {
//    send_by_name(vm, MRBC_SYM(EQ_EQ), a, 1);
//    return;
//  }

  result = mrbc_compare(&regs[a], &regs[a+1]);

  mrbc_decref(&regs[a]);
  regs[a].tt = result ? MRBC_TT_FALSE : MRBC_TT_TRUE;
}
static void op_lt( mrbc_vm *vm, mrbc_value *regs )
{
  int result;
  FETCH_B();

//  if (regs[a].tt == MRBC_TT_OBJECT) {
//    send_by_name(vm, MRBC_SYM(LT), a, 1);
//    return;
//  }

  result = mrbc_compare(&regs[a], &regs[a+1]);

  mrbc_decref(&regs[a]);
  regs[a].tt = result < 0 ? MRBC_TT_TRUE : MRBC_TT_FALSE;
}
static void op_le( mrbc_vm *vm, mrbc_value *regs )
{
  int result;
  FETCH_B();

//  if (regs[a].tt == MRBC_TT_OBJECT) {
//    send_by_name(vm, MRBC_SYM(LT_EQ), a, 1);
//    return;
//  }

  result = mrbc_compare(&regs[a], &regs[a+1]);

  mrbc_decref(&regs[a]);
  regs[a].tt = result <= 0 ? MRBC_TT_TRUE : MRBC_TT_FALSE;
}
static void op_gt( mrbc_vm *vm, mrbc_value *regs )
{
  int result;
  FETCH_B();

//  if (regs[a].tt == MRBC_TT_OBJECT) {
//    send_by_name(vm, MRBC_SYM(GT), a, 1);
//    return;
//  }

  result = mrbc_compare(&regs[a], &regs[a+1]);

  mrbc_decref(&regs[a]);
  regs[a].tt = result > 0 ? MRBC_TT_TRUE : MRBC_TT_FALSE;
}
static void op_ge( mrbc_vm *vm, mrbc_value *regs )
{
  int result;
  FETCH_B();

//  if (regs[a].tt == MRBC_TT_OBJECT) {
//    send_by_name(vm, MRBC_SYM(GT_EQ), a, 1);
//    return;
//  }

  result = mrbc_compare(&regs[a], &regs[a+1]);

  mrbc_decref(&regs[a]);
  regs[a].tt = result >= 0 ? MRBC_TT_TRUE : MRBC_TT_FALSE;
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
    //while(1);
    //for(i=0;i<10000;i++);
    switch (op) {
      case OP_NOP:        op_nop        (vm, regs ); break;
      case OP_MOVE:       op_move       (vm, regs ); break;
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

      case OP_JMPNOT:     op_jmpnot     (vm, regs); break;

      case OP_SSEND:      op_ssend      (vm, regs); break;

      case OP_RETURN: op_return(vm, regs); break;

      case OP_ADD:        op_add        (vm, regs); break;
      case OP_ADDI:       op_addi       (vm, regs); break;
      case OP_SUB:        op_sub        (vm, regs); break;
      case OP_SUBI:       op_subi       (vm, regs); break;

      case OP_EQ:         op_eq         (vm, regs ); break;
      case OP_LT:         op_lt         (vm, regs); break;
      case OP_LE:         op_le         (vm, regs); break;
      case OP_GT:         op_gt         (vm, regs); break;
      case OP_GE:         op_ge         (vm, regs); break;

      case OP_STOP: op_stop(vm, regs); break;
      default:
        panic("UNKNOWN OPCODE");
        break;
    }
    if (op == OP_STOP) break;
  }
}
