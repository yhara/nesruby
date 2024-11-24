#include <string.h> // strlen, memcmp
#include "symbol.h"
#include "value.h"

int mrbc_compare(const mrbc_value *v1, const mrbc_value *v2)
{
  // if TT_XXX is different
  if( mrbc_type(*v1) != mrbc_type(*v2) ) {
    // leak Empty?
    if((mrbc_type(*v1) == MRBC_TT_EMPTY && mrbc_type(*v2) == MRBC_TT_NIL) ||
       (mrbc_type(*v1) == MRBC_TT_NIL   && mrbc_type(*v2) == MRBC_TT_EMPTY)) return 0;

    // other case
    return mrbc_type(*v1) - mrbc_type(*v2);
  }

  // check value
  switch( mrbc_type(*v1) ) {
  case MRBC_TT_NIL:
  case MRBC_TT_FALSE:
  case MRBC_TT_TRUE:
    return 0;

  case MRBC_TT_INTEGER:
    return mrbc_integer(*v1) - mrbc_integer(*v2);

  case MRBC_TT_SYMBOL: {
    const char *str1 = mrbc_symid_to_str(mrbc_symbol(*v1));
    const char *str2 = mrbc_symid_to_str(mrbc_symbol(*v2));
    int diff = strlen(str1) - strlen(str2);
    int len = diff < 0 ? strlen(str1) : strlen(str2);
    int res = memcmp(str1, str2, len);
    return (res != 0) ? res : diff;
  }

//  case MRBC_TT_CLASS:
//  case MRBC_TT_OBJECT:
//  case MRBC_TT_PROC:
//    return (v1->cls > v2->cls) * 2 - (v1->cls != v2->cls);
//
//  case MRBC_TT_ARRAY:
//    return mrbc_array_compare( v1, v2 );
//
//#if MRBC_USE_STRING
//  case MRBC_TT_STRING:
//    return mrbc_string_compare( v1, v2 );
//#endif
//
//  case MRBC_TT_RANGE:
//    return mrbc_range_compare( v1, v2 );
//
//  case MRBC_TT_HASH:
//    return mrbc_hash_compare( v1, v2 );

  default:
    return 1;
  }

#if MRBC_USE_FLOAT
 CMP_FLOAT:
  return -1 + (d1 == d2) + (d1 > d2)*2;	// caution: NaN == NaN is false
#endif
}
