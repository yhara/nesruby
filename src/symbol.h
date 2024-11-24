#ifndef MRBC_SRC_SYMBOL_H_
#define MRBC_SRC_SYMBOL_H_

#include "value.h"

mrbc_sym mrbc_str_to_symid(const char *str);
const char *mrbc_symid_to_str(mrbc_sym sym_id);

#endif
