#ifndef MRBC_SRC_VM_CONFIG_H_
#define MRBC_SRC_VM_CONFIG_H_

// maximum number of user(=non built-in) symbols
#if !defined(MAX_SYMBOLS_COUNT)
#define MAX_SYMBOLS_COUNT 40
#endif

// TODO: when this value is too big, mrbc_regs is silently broken
#define MRBC_MEMORY_SIZE 724

#define MAX_REGS 40

#endif // MRBC_SRC_VM_CONFIG_H_
