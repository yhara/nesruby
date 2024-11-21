#ifndef MRBC_SRC_AUTOGEN_BUILTIN_SYMBOL_H_
#define MRBC_SRC_AUTOGEN_BUILTIN_SYMBOL_H_

static const char *builtin_symbols[] = {
  "hello",		// MRBC_SYMID_hello = 0(0x0)
  "bye",		// MRBC_SYMID_bye = 1(0x1)
};
enum {
  MRBC_SYMID_hello = 0,
  MRBC_SYMID_bye = 1,
};

#define MRBC_SYM(sym) MRBC_SYMID_##sym
#endif
