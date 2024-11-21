#include <string.h> // for strcmp
#include "vm_config.h"
#include "symbol.h"

#define MRBC_SYMBOL_TABLE_INDEX_TYPE	uint8_t

struct SYM_INDEX {
  uint16_t hash;	//!< hash value, returned by calc_hash().
  const char *cstr;	//!< point to the symbol string.
};

static struct SYM_INDEX sym_index[MAX_SYMBOLS_COUNT];
static int sym_index_pos;	// point to the last(free) sym_index array.

static uint16_t calc_hash(const char *str)
{
  uint16_t h = 0;

  while( *str != '\0' ) {
    h = h * 17 + *str++;
  }
  return h;
}

static int search_index( uint16_t hash, const char *str )
{
  int i;
  for( i = 0; i < sym_index_pos; i++ ) {
    if( sym_index[i].hash == hash && strcmp(str, sym_index[i].cstr) == 0 ) {
      return i;
    }
  }
  return -1;
}

static int add_index( uint16_t hash, const char *str )
{
  int idx;
  if( sym_index_pos >= MAX_SYMBOLS_COUNT ) return -1;	// check overflow.

  idx = sym_index_pos++;

  // append table.
  sym_index[idx].hash = hash;
  sym_index[idx].cstr = str;

  return idx;
}

/***** Global functions *****************************************************/

void mrbc_cleanup_symbol(void)
{
  memset(sym_index, 0, sizeof(sym_index));
  sym_index_pos = 0;
}

mrbc_sym mrbc_str_to_symid(const char *str)
{
  mrbc_sym sym_id;
  //TODO
  //sym_id = search_builtin_symbol(str);
  //if( sym_id >= 0 ) return sym_id;

  uint16_t h = calc_hash(str);
  sym_id = search_index(h, str);
  if( sym_id < 0 ) sym_id = add_index( h, str );
  if( sym_id < 0 ) return sym_id;

  return sym_id; // + OFFSET_BUILTIN_SYMBOL;
}
