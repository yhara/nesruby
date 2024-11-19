#include "neslib.h"
#include "src/mrb_data.h"
#include "src/opcode.h"
#include "src/shims.h"
#include "src/value.h"
#include "src/vm.h"
#include "src/debug.h"

//palette for balls, there are four sets for different ball colors
const unsigned char palSprites[16]={
  0x0f,0x17,0x27,0x37,
  0x0f,0x11,0x21,0x31,
  0x0f,0x15,0x25,0x35,
  0x0f,0x19,0x29,0x39
};

static mrbc_vm vm;
static const uint8_t MAX_REGS_SIZE = 16;
static mrbc_value mrbc_regs[16];

static const int SIZE_RITE_BINARY_HEADER = 20;
static const int SIZE_RITE_SECTION_HEADER = 12;
static const char IREP[4] = "IREP";
void run_ruby()
{
  uint8_t *p = (uint8_t *)mrb_data;
  uint16_t ilen;
  uint16_t clen;
  // Load .mrb
  p += SIZE_RITE_BINARY_HEADER;
  // Load IREP section
  p += SIZE_RITE_SECTION_HEADER;
  // Load an IREP
  p += 4; // skip record size
  p += 2; // nlocals
  p += 2; // nregs
  p += 2; // rlen
  clen = bin_to_uint16(p); p += 2;
  p += 2; ilen = bin_to_uint16(p); p += 2;
  // Load an ISEQ
  vm.inst = p;

  vm.cur_regs = mrbc_regs;
  mrbc_vm_run(&vm);
}

const static char SPR_ARROW = 0;
const static char SPR_RUBY = 4;

void main(void)
{
  unsigned char input;
  signed char dx;
  signed int x;

  //rendering is disabled at the startup, the palette is all black

  pal_col(1,0x30);//set while color

  pal_spr(palSprites);//set palette for sprites

  //you can't put data into vram through vram_put while rendering is enabled
  //so you have to disable rendering to put things like text or a level map
  //into the nametable

  //there is a way to update small number of nametable tiles while rendering
  //is enabled, using set_vram_update and an update list

  //run_ruby();

  put_str(NTADR_A(2,2),"HELLO, WORLD,");

  ppu_on_all();//enable rendering

  x = 0;
  dx = 1;
  while(1)
  {
    ppu_wait_frame();//wait for next TV frame

    input = pad_trigger(0);
    if(input&PAD_A)
    {
      sfx_play(3,3);
    }

    //for (n = 0; n < 32; n++) {
    //  spr = oam_spr(n*8, n/2, 0x40, 1, spr);
    //}
    x += dx;
    if (x < 0 || x > 256-8) {
      dx = -dx;
      x += dx;
    }
    oam_spr(x, 8, 0x45, 1, SPR_ARROW);

    oam_spr(200, 200, 0x46, 2, SPR_RUBY);
  }
}

