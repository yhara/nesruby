#ifndef MRBC_SRC_VM_H_
#define MRBC_SRC_VM_H_

#include "shims.h"
#include "value.h"

typedef struct IREP {
  uint16_t nlocals;		//!< num of local variables
  uint16_t nregs;		//!< num of register variables
  uint16_t rlen;		//!< num of child IREP blocks
  uint16_t clen;		//!< num of catch handlers
  // NOTE: ilen is 32bit in mruby/c
  uint16_t ilen;		//!< num of bytes in OpCode
  uint16_t plen;		//!< num of pools
  uint16_t slen;		//!< num of symbols
  uint16_t ofs_ireps;		//!< offset of data->tbl_ireps. (32bit aligned)

  const uint8_t *inst;		//!< pointer to instruction in RITE binary
  const uint8_t *pool;		//!< pointer to pool in RITE binary

  uint8_t data[];		//!< variable data. (see load.c)
				//!<  mrbc_sym   tbl_syms[slen]
				//!<  uint16_t   tbl_pools[plen]
				//!<  mrbc_irep *tbl_ireps[rlen]
} mrbc_irep;
typedef struct IREP mrb_irep;

// mrbc_irep manipulate macro.
//! get a symbol id table pointer.
#define mrbc_irep_tbl_syms(irep)	((mrbc_sym *)(irep)->data)
//! get a n'th symbol id in irep
#define mrbc_irep_symbol_id(irep, n)	mrbc_irep_tbl_syms(irep)[(n)]

//! get a child irep table pointer.
#define mrbc_irep_tbl_ireps(irep) \
  ( (mrbc_irep **) ((irep)->data + (irep)->ofs_ireps * 4) )

typedef struct VM {

  mrbc_irep       *top_irep;		//!< IREP tree top.
  const mrbc_irep *cur_irep;		//!< IREP currently running.
  uint8_t *inst;
  mrbc_value	  *cur_regs;		//!< Current register top.
                                        //
} mrbc_vm;

void mrbc_vm_run(mrbc_vm *vm);

static uint16_t bin_to_uint16( const void *s )
{
  // Little endian, no alignment.
  uint16_t x = *((uint16_t *)s);
  x = (x << 8) | (x >> 8);
  return x;
}

#endif

