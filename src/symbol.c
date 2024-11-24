#include <string.h> // for strcmp
#include "_autogen_builtin_symbol.h"
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

static int search_builtin_symbol( const char *str )
{
  int left = 0;
  int mid;
  int right = sizeof(builtin_symbols) / sizeof(builtin_symbols[0]);
  const unsigned char *p1, *p2;

  while( left < right ) {
    mid = (left + right) / 2;
    p1 = (const unsigned char *)builtin_symbols[mid];
    p2 = (const unsigned char *)str;

    while( 1 ) {	// string compare, same order as cruby.
      if( *p1 < *p2 ) {
	left = mid + 1;
	break;
      }
      if( *p1 > *p2 ) {
	right = mid;
	break;
      }
      if( *p1 == 0 ) {
	return mid;
      }

      p1++;
      p2++;
    }
  }

  return -1;
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
  uint16_t h;
  mrbc_sym sym_id;
  sym_id = search_builtin_symbol(str);
  if( sym_id >= 0 ) return sym_id;

  h = calc_hash(str);
  sym_id = search_index(h, str);
  if( sym_id < 0 ) sym_id = add_index( h, str );
  if( sym_id < 0 ) return sym_id;

  return sym_id; + OFFSET_BUILTIN_SYMBOL;
}

const char * mrbc_symid_to_str(mrbc_sym sym_id)
{
  if( sym_id < OFFSET_BUILTIN_SYMBOL ) {
    return builtin_symbols[sym_id];
  }

  sym_id -= OFFSET_BUILTIN_SYMBOL;
  if( sym_id < 0 ) return NULL;
  if( sym_id >= sym_index_pos ) return NULL;

  return sym_index[sym_id].cstr;
}
