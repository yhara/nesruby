#include <conio.h>
#include <string.h>
// cc65 memo
// - no "inline" keyword
// limitation
// - ilen must be 16bit

typedef unsigned char uint8_t;
typedef unsigned uint16_t;
//typedef struct {
//    uint16_t low;
//    uint16_t high;
//} uint32_t;

static uint16_t bin_to_uint16( const void *s )
{
  uint16_t x = *((uint16_t *)s);
  x = (x << 8) | (x >> 8);
  return x;
}
//static uint32_t bin_to_uint32( const void *s )
//{
//  uint32_t x = *((uint32_t *)s);
//  x = (x << 24) | ((x & 0xff00) << 8) | ((x >> 8) & 0xff00) | (x >> 24);
//  return x;
//}

typedef struct {
  uint8_t *inst;
} mrb_vm;
static mrb_vm vm;

enum OPCODE {
/*-----------------------------------------------------------------------
  operation code        operands  semantics
------------------------------------------------------------------------*/
  OP_NOP        = 0x00, //!< Z    no operation
  OP_MOVE       = 0x01, //!< BB   R[a] = R[b]
  OP_LOADL      = 0x02, //!< BB   R[a] = Pool[b]
  OP_LOADI      = 0x03, //!< BB   R[a] = mrb_int(b)
  OP_LOADINEG   = 0x04, //!< BB   R[a] = mrb_int(-b)
  OP_LOADI__1   = 0x05, //!< B    R[a] = mrb_int(-1)
  OP_LOADI_0    = 0x06, //!< B    R[a] = mrb_int(0)
  OP_LOADI_1    = 0x07, //!< B    R[a] = mrb_int(1)
  OP_LOADI_2    = 0x08, //!< B    R[a] = mrb_int(2)
  OP_LOADI_3    = 0x09, //!< B    R[a] = mrb_int(3)
  OP_LOADI_4    = 0x0A, //!< B    R[a] = mrb_int(4)
  OP_LOADI_5    = 0x0B, //!< B    R[a] = mrb_int(5)
  OP_LOADI_6    = 0x0C, //!< B    R[a] = mrb_int(6)
  OP_LOADI_7    = 0x0D, //!< B    R[a] = mrb_int(7)
  OP_LOADI16    = 0x0E, //!< BS   R[a] = mrb_int(b)
  OP_LOADI32    = 0x0F, //!< BSS  R[a] = mrb_int((b<<16)+c)
  OP_LOADSYM    = 0x10, //!< BB   R[a] = Syms[b]
  OP_LOADNIL    = 0x11, //!< B    R[a] = nil
  OP_LOADSELF   = 0x12, //!< B    R[a] = self
  OP_LOADT      = 0x13, //!< B    R[a] = true
  OP_LOADF      = 0x14, //!< B    R[a] = false
  OP_GETGV      = 0x15, //!< BB   R[a] = getglobal(Syms[b])
  OP_SETGV      = 0x16, //!< BB   setglobal(Syms[b], R[a])
  OP_GETSV      = 0x17, //!< BB   R[a] = Special[Syms[b]]
  OP_SETSV      = 0x18, //!< BB   Special[Syms[b]] = R[a]
  OP_GETIV      = 0x19, //!< BB   R[a] = ivget(Syms[b])
  OP_SETIV      = 0x1A, //!< BB   ivset(Syms[b],R[a])
  OP_GETCV      = 0x1B, //!< BB   R[a] = cvget(Syms[b])
  OP_SETCV      = 0x1C, //!< BB   cvset(Syms[b],R[a])
  OP_GETCONST   = 0x1D, //!< BB   R[a] = constget(Syms[b])
  OP_SETCONST   = 0x1E, //!< BB   constset(Syms[b],R[a])
  OP_GETMCNST   = 0x1F, //!< BB   R[a] = R[a]::Syms[b]
  OP_SETMCNST   = 0x20, //!< BB   R[a+1]::Syms[b] = R[a]
  OP_GETUPVAR   = 0x21, //!< BBB  R[a] = uvget(b,c)
  OP_SETUPVAR   = 0x22, //!< BBB  uvset(b,c,R[a])
  OP_GETIDX     = 0x23, //!< B    R[a] = R[a][R[a+1]]
  OP_SETIDX     = 0x24, //!< B    R[a][R[a+1]] = R[a+2]
  OP_JMP        = 0x25, //!< S    pc+=a
  OP_JMPIF      = 0x26, //!< BS   if R[a] pc+=b
  OP_JMPNOT     = 0x27, //!< BS   if !R[a] pc+=b
  OP_JMPNIL     = 0x28, //!< BS   if R[a]==nil pc+=b
  OP_JMPUW      = 0x29, //!< S    unwind_and_jump_to(a)
  OP_EXCEPT     = 0x2A, //!< B    R[a] = exc
  OP_RESCUE     = 0x2B, //!< BB   R[b] = R[a].isa?(R[b])
  OP_RAISEIF    = 0x2C, //!< B    raise(R[a]) if R[a]
  OP_SSEND      = 0x2D, //!< BBB  R[a] = self.send(Syms[b],R[a+1]..,R[a+n+1]:R[a+n+2]..) (c=n|k<<4)
  OP_SSENDB     = 0x2E, //!< BBB  R[a] = self.send(Syms[b],R[a+1]..,R[a+n+1]:R[a+n+2]..,&R[a+n+2k+1])
  OP_SEND       = 0x2F, //!< BBB  R[a] = R[a].send(Syms[b],R[a+1]..,R[a+n+1]:R[a+n+2]..) (c=n|k<<4)
  OP_SENDB      = 0x30, //!< BBB  R[a] = R[a].send(Syms[b],R[a+1]..,R[a+n+1]:R[a+n+2]..,&R[a+n+2k+1])
  OP_CALL       = 0x31, //!< Z    R[0] = self.call(frame.argc, frame.argv)
  OP_SUPER      = 0x32, //!< BB   R[a] = super(R[a+1],... ,R[a+b+1])
  OP_ARGARY     = 0x33, //!< BS   R[a] = argument array (16=m5:r1:m5:d1:lv4)
  OP_ENTER      = 0x34, //!< W    arg setup according to flags (23=m5:o5:r1:m5:k5:d1:b1)
  OP_KEY_P      = 0x35, //!< BB   R[a] = kdict.key?(Syms[b])
  OP_KEYEND     = 0x36, //!< Z    raise unless kdict.empty?
  OP_KARG       = 0x37, //!< BB   R[a] = kdict[Syms[b]]; kdict.delete(Syms[b])
  OP_RETURN     = 0x38, //!< B    return R[a] (normal)
  OP_RETURN_BLK = 0x39, //!< B    return R[a] (in-block return)
  OP_BREAK      = 0x3A, //!< B    break R[a]
  OP_BLKPUSH    = 0x3B, //!< BS   R[a] = block (16=m5:r1:m5:d1:lv4)
  OP_ADD        = 0x3C, //!< B    R[a] = R[a]+R[a+1]
  OP_ADDI       = 0x3D, //!< BB   R[a] = R[a]+mrb_int(b)
  OP_SUB        = 0x3E, //!< B    R[a] = R[a]-R[a+1]
  OP_SUBI       = 0x3F, //!< BB   R[a] = R[a]-mrb_int(b)
  OP_MUL        = 0x40, //!< B    R[a] = R[a]*R[a+1]
  OP_DIV        = 0x41, //!< B    R[a] = R[a]/R[a+1]
  OP_EQ         = 0x42, //!< B    R[a] = R[a]==R[a+1]
  OP_LT         = 0x43, //!< B    R[a] = R[a]<R[a+1]
  OP_LE         = 0x44, //!< B    R[a] = R[a]<=R[a+1]
  OP_GT         = 0x45, //!< B    R[a] = R[a]>R[a+1]
  OP_GE         = 0x46, //!< B    R[a] = R[a]>=R[a+1]
  OP_ARRAY      = 0x47, //!< BB   R[a] = ary_new(R[a],R[a+1]..R[a+b])
  OP_ARRAY2     = 0x48, //!< BBB  R[a] = ary_new(R[b],R[b+1]..R[b+c])
  OP_ARYCAT     = 0x49, //!< B    ary_cat(R[a],R[a+1])
  OP_ARYPUSH    = 0x4A, //!< BB   ary_push(R[a],R[a+1]..R[a+b])
  OP_ARYDUP     = 0x4B, //!< B    R[a] = ary_dup(R[a])
  OP_AREF       = 0x4C, //!< BBB  R[a] = R[b][c]
  OP_ASET       = 0x4D, //!< BBB  R[b][c] = R[a]
  OP_APOST      = 0x4E, //!< BBB  *R[a],R[a+1]..R[a+c] = R[a][b..]
  OP_INTERN     = 0x4F, //!< B    R[a] = intern(R[a])
  OP_SYMBOL     = 0x50, //!< BB   R[a] = intern(Pool[b])
  OP_STRING     = 0x51, //!< BB   R[a] = str_dup(Pool[b])
  OP_STRCAT     = 0x52, //!< B    str_cat(R[a],R[a+1])
  OP_HASH       = 0x53, //!< BB   R[a] = hash_new(R[a],R[a+1]..R[a+b*2-1])
  OP_HASHADD    = 0x54, //!< BB   hash_push(R[a],R[a+1]..R[a+b*2])
  OP_HASHCAT    = 0x55, //!< B    R[a] = hash_cat(R[a],R[a+1])
  OP_LAMBDA     = 0x56, //!< BB   R[a] = lambda(Irep[b],L_LAMBDA)
  OP_BLOCK      = 0x57, //!< BB   R[a] = lambda(Irep[b],L_BLOCK)
  OP_METHOD     = 0x58, //!< BB   R[a] = lambda(Irep[b],L_METHOD)
  OP_RANGE_INC  = 0x59, //!< B    R[a] = range_new(R[a],R[a+1],FALSE)
  OP_RANGE_EXC  = 0x5A, //!< B    R[a] = range_new(R[a],R[a+1],TRUE)
  OP_OCLASS     = 0x5B, //!< B    R[a] = ::Object
  OP_CLASS      = 0x5C, //!< BB   R[a] = newclass(R[a],Syms[b],R[a+1])
  OP_MODULE     = 0x5D, //!< BB   R[a] = newmodule(R[a],Syms[b])
  OP_EXEC       = 0x5E, //!< BB   R[a] = blockexec(R[a],Irep[b])
  OP_DEF        = 0x5F, //!< BB   R[a].newmethod(Syms[b],R[a+1]); R[a] = Syms[b]
  OP_ALIAS      = 0x60, //!< BB   alias_method(target_class,Syms[a],Syms[b])
  OP_UNDEF      = 0x61, //!< B    undef_method(target_class,Syms[a])
  OP_SCLASS     = 0x62, //!< B    R[a] = R[a].singleton_class
  OP_TCLASS     = 0x63, //!< B    R[a] = target_class
  OP_DEBUG      = 0x64, //!< BBB  print a,b,c
  OP_ERR        = 0x65, //!< B    raise(LocalJumpError, Pool[a])
  OP_EXT1       = 0x66, //!< Z    make 1st operand (a) 16bit
  OP_EXT2       = 0x67, //!< Z    make 2nd operand (b) 16bit
  OP_EXT3       = 0x68, //!< Z    make 1st and 2nd operands 16bit
  OP_STOP       = 0x69, //!< Z    stop VM
};

void run_ruby();
static const int SIZE_RITE_BINARY_HEADER = 20;
static const int SIZE_RITE_SECTION_HEADER = 12;
static const char IREP[4] = "IREP";

const char message[] = "Hello cc65!";

const uint8_t mrb_data[]={
82,73,84,69,48,51,48,48,0,0,0,66,77,65,84,90,48,48,48,48,73,82,69,80,0,0,0,38,48,51,48,48,0,0,0,26,0,1,0,4,0,0,0,0,0,0,0,6,3,1,30,56,1,105,0,0,0,0,69,78,68,0,0,0,0,8
};

// vm
static const uint8_t MAX_REGS_SIZE = 16;
static uint8_t mrb_regs[16];

int main(void)
{
    unsigned int frame = 0;

    clrscr();
    //gotoxy((32-strlen(message))/2, 30/2);
    gotoxy(0, 15);
    run_ruby();
    //cprintf("value: %03d", mrb_data[0]);
    while(1) {
        gotoxy(0, 0);
        cprintf("%8d", frame++);
    }

    return 0;
}

#define FETCH_B() \
  unsigned int a; \
  a = *vm->inst++; \
  (void)a
#define FETCH_BB() \
  unsigned int a, b; \
  a = *vm->inst++; \
  b = *vm->inst++; \
  (void)a, (void)b

static void op_loadi(mrb_vm *vm)
{
  FETCH_BB();
  mrb_regs[a] = b;
}
static void op_return(mrb_vm *vm)
{
  FETCH_B();
  cprintf("answer is %d", mrb_regs[a]);
}
static void op_stop(mrb_vm *vm)
{
  //cprintf("stop");
}

static char buf[10];
void run_ruby()
{
  uint8_t *p = (uint8_t *)mrb_data;
  uint8_t op;
  uint16_t ilen;
  uint16_t clen;
  // Load .mrb
  p += SIZE_RITE_BINARY_HEADER;
  //cprintf("%3s", (char *)mrb_data);
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
  while (1) {
    op = *vm.inst++;
    switch (op) {
      case OP_LOADI: op_loadi(&vm); break;
      case OP_RETURN: op_return(&vm); break;
      case OP_STOP: op_stop(&vm); break;
      default:
        cprintf("Unknown opcode: %d,%d,%d", op,clen,ilen);
        break;
    }
    if (op == OP_STOP) break;
  }
}
