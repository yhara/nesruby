#ifndef MRBC_SRC_VALUE_H_
#define MRBC_SRC_VALUE_H_

#include "shims.h"

typedef int16_t mrbc_int_t;
typedef uint16_t mrbc_uint_t;
typedef int16_t mrbc_sym;	//!< mruby/c symbol ID

typedef enum {
  MRBC_TT_EMPTY     = 0,
  MRBC_TT_NIL       = 1,        //!< NilClass
  MRBC_TT_FALSE     = 2,        //!< FalseClass
                                //
  MRBC_TT_TRUE      = 3,        //!< TrueClass
  MRBC_TT_INTEGER   = 4,        //!< Integer
  MRBC_TT_SYMBOL    = 6,        //!< Symbol
} mrbc_vtype;

struct RObject {
  mrbc_vtype tt : 8;
  union {
    mrbc_int_t i;		// MRBC_TT_INTEGER, SYMBOL
  };
};

typedef struct RObject mrbc_value;

// setters
#define mrbc_set_integer(p,n)	(p)->tt = MRBC_TT_INTEGER; (p)->i = (n)
#define mrbc_set_nil(p)		(p)->tt = MRBC_TT_NIL
#define mrbc_set_true(p)	(p)->tt = MRBC_TT_TRUE
#define mrbc_set_false(p)	(p)->tt = MRBC_TT_FALSE
#define mrbc_set_bool(p,n)	(p)->tt = (n)? MRBC_TT_TRUE: MRBC_TT_FALSE
#define mrbc_set_symbol(p,n)	(p)->tt = MRBC_TT_SYMBOL; (p)->i = (n)

static void mrbc_decref(mrbc_value *v)
{
  // todo
  (void)v; // Surpress "never used" warning
}
static void mrbc_incref(mrbc_value *v)
{
  // todo
  (void)v; // Surpress "never used" warning
}

#endif
