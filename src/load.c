#include <string.h> // for memcmp
#include "debug.h"
#include "shims.h"
#include "symbol.h"
#include "vm.h"
#include "value.h"

static const int SIZE_RITE_BINARY_HEADER = 20;
static const int SIZE_RITE_SECTION_HEADER = 12;
static const int SIZE_RITE_CATCH_HANDLER = 13;
static const char IREP[4] = "IREP";
static const char END[4] = "END\0";
enum irep_pool_type {
  IREP_TT_STR   = 0,	// string (need free)
  IREP_TT_SSTR  = 2,	// string (static)
  IREP_TT_INT32 = 1,	// 32bit integer
  IREP_TT_INT64 = 3,	// 64bit integer
  IREP_TT_FLOAT = 5,	// float (double/float)
};

static mrbc_irep * load_irep_1(struct VM *vm, const uint8_t *bin, int *len, int flag_top)
{
  mrbc_irep irep;
  int i, siz;
  const uint8_t *p = bin + 4;	// 4 = skip record size.
  mrbc_sym *tbl_syms;
  char *sym_str;
  mrbc_sym sym;
  const uint8_t *inst2;
  (void)len;
  (void)flag_top;

  irep.nlocals = bin_to_uint16(p);	p += 2;
  irep.nregs = bin_to_uint16(p);	p += 2;
  irep.rlen = bin_to_uint16(p);		p += 2;
  irep.clen = bin_to_uint16(p);		p += 2;
  p += 2; irep.ilen = bin_to_uint16(p);		p += 2;
  irep.inst = p;

  // POOL block
  p += irep.ilen + SIZE_RITE_CATCH_HANDLER * irep.clen;
  irep.pool = p;
  irep.plen = bin_to_uint16(p);		p += 2;

  // skip pool
  for( i = 0; i < irep.plen; i++ ) {
    siz = 0;
    switch( *p++ ) {
    case IREP_TT_STR:
    case IREP_TT_SSTR:	siz = bin_to_uint16(p) + 3;	break;
    default:
      panic("LORDER: UNKNOWN TT");
      return NULL;
    }
    p += siz;
  }

  // num of symbols, offset of tbl_ireps.
  irep.slen = bin_to_uint16(p);		p += 2;
  siz = sizeof(mrbc_sym) * irep.slen + sizeof(uint16_t) * irep.plen;
  siz += (-siz & 0x03);	// padding. 32bit align.
  irep.ofs_ireps = siz >> 2;

  // TODO: allocate new irep

  // make a sym_id table.
  tbl_syms = mrbc_irep_tbl_syms(&irep);
  for( i = 0; i < irep.slen; i++ ) {
    siz = bin_to_uint16(p) + 1;	p += 2;
    sym_str = (char *)p;
    sym = mrbc_str_to_symid( sym_str );
    if( sym < 0 ) {
      panic("LORDER: TOO MANY SYMBOLS");
      return NULL;
    }
    *tbl_syms++ = sym;
    p += (siz);
  }

  vm->cur_irep = &irep;
  vm->inst = (uint8_t *)irep.inst;

  // TEMP: Run the VM from here
  mrbc_vm_run(vm);

  return 0;
}

static mrbc_irep *load_irep(struct VM *vm, const uint8_t *bin, int *len)
{
  int len1, total_len, i;
  mrbc_irep **tbl_ireps;
  mrbc_irep *irep = load_irep_1(vm, bin, &len1, len == 0);
  if( !irep ) return NULL;
  total_len = len1;

  tbl_ireps = mrbc_irep_tbl_ireps(irep);
  for( i = 0; i < irep->rlen; i++ ) {
    tbl_ireps[i] = load_irep(vm, bin + total_len, &len1);
    if( ! tbl_ireps[i] ) return NULL;
    total_len += len1;
  }

  if( len ) *len = total_len;
  return irep;
}

int mrbc_load_irep(struct VM *vm, const void *bytecode)
{
  const uint8_t *bin = bytecode;

  vm->top_irep = load_irep( vm, bin + SIZE_RITE_SECTION_HEADER, 0 );
  if( vm->top_irep == NULL ) return -1;

  return 0; //mrbc_israised(vm);
}

int mrbc_load_mrb(struct VM *vm, const void *bytecode)
{
  const uint8_t *bin = bytecode;
  bin += SIZE_RITE_BINARY_HEADER;

  while( 1 ) {
    if( memcmp(bin, IREP, sizeof(IREP)) == 0 ) {
      if( mrbc_load_irep( vm, bin ) != 0 ) break;

    } else if( memcmp(bin, END, sizeof(END)) == 0 ) {
      break;
    }
    // ignore other section.

    bin += 4;
    bin += 2;
    bin += bin_to_uint16(bin);
  }

  return 0; //mrbc_israised(vm);
}
