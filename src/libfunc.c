#include "opcode.h"
#include "vm.h"
#include "debug.h"

#ifdef __CC65__
  #include "neslib.h"
  const static char SPR_ARROW = 0;
  const static char SPR_RUBY = 4;
#endif

void call_libfunc( mrbc_vm *vm, mrbc_value *regs)
{
  int rd;
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
#ifdef __CC65__
    case MRBC_SYM(wait_frame):
      ppu_wait_frame();
      break;
    case MRBC_SYM(rand8):
      rd = rand8();
      //d_putd(rd); d_puts("");
      mrbc_set_integer(&regs[a], rd);
      break;
    case MRBC_SYM(draw_arrow):
      oam_spr(regs[a+1].i,
              regs[a+2].i, 0x45, 1, SPR_ARROW);
      break;
    case MRBC_SYM(draw_ruby):
      oam_spr(regs[a+1].i,
              regs[a+2].i, 0x46, 2, SPR_RUBY);
      break;
    case MRBC_SYM(play_music):
      music_play(regs[a+1].i);
      break;
    case MRBC_SYM(stop_music):
      music_stop();
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
#endif

    case MRBC_SYM(dbg):
      d_putd(regs[a+1].i); d_puts("");
      break;
    default:
      panic("UNKNOWN METHOD");
  }
}
