// Generated by gen_builtin_symbols.rb
#ifndef MRBC_SRC_AUTOGEN_BUILTIN_SYMBOL_H_
#define MRBC_SRC_AUTOGEN_BUILTIN_SYMBOL_H_
static const char *builtin_symbols[] = {
  "btn_a_pressed",  // MRBC_SYMID_btn_a_pressed = 0(0x0)
  "dbg",  // MRBC_SYMID_dbg = 1(0x1)
  "draw_arrow",  // MRBC_SYMID_draw_arrow = 2(0x2)
  "draw_ruby",  // MRBC_SYMID_draw_ruby = 3(0x3)
  "hide_title",  // MRBC_SYMID_hide_title = 4(0x4)
  "pad_trigger",  // MRBC_SYMID_pad_trigger = 5(0x5)
  "play_music",  // MRBC_SYMID_play_music = 6(0x6)
  "play_sound",  // MRBC_SYMID_play_sound = 7(0x7)
  "rand8",  // MRBC_SYMID_rand8 = 8(0x8)
  "show_title",  // MRBC_SYMID_show_title = 9(0x9)
  "stop_music",  // MRBC_SYMID_stop_music = 10(0xa)
  "wait_frame",  // MRBC_SYMID_wait_frame = 11(0xb)
};
enum {
  MRBC_SYMID_btn_a_pressed = 0,
  MRBC_SYMID_dbg = 1,
  MRBC_SYMID_draw_arrow = 2,
  MRBC_SYMID_draw_ruby = 3,
  MRBC_SYMID_hide_title = 4,
  MRBC_SYMID_pad_trigger = 5,
  MRBC_SYMID_play_music = 6,
  MRBC_SYMID_play_sound = 7,
  MRBC_SYMID_rand8 = 8,
  MRBC_SYMID_show_title = 9,
  MRBC_SYMID_stop_music = 10,
  MRBC_SYMID_wait_frame = 11,
};
#define MRBC_SYM(sym) MRBC_SYMID_##sym
#define OFFSET_BUILTIN_SYMBOL 12
#endif
