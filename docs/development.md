---
layout: page
title: Development
permalink: /development/
---

# Development Guide

## Architecture Overview

nesruby is built as a custom virtual machine based on [mruby/c](https://github.com/mrubyc/mrubyc), optimized for NES hardware constraints.

### Core Components

**Virtual Machine Core:**
- `src/vm.c`, `src/vm.h` - Main VM implementation
- `src/vm_config.h` - Memory and resource limits
- `src/load.c` - Bytecode loading and initialization

**Value System:**
- `src/value.c`, `src/value.h` - Ruby value representation
- Support for nil, boolean, integer, and symbol types

**Runtime Support:**
- `src/symbol.c` - Symbol table management
- `src/libfunc.c` - Built-in Ruby method implementations

**NES Integration:**
- `src/main.c` - NES-specific initialization and main loop
- PPU, sprite, and palette setup
- NROM mapper configuration (`nrom_256_horz.cfg`)

## Build System

### Rake Tasks

The project uses Rake with several important targets:

```ruby
# Main build tasks
rake default    # Build bin/nesruby.nes
rake :nones_test # Run test suite
```

### Compilation Pipeline

1. **Ruby → Bytecode:** `mrbc` compiles `.rb` to `.mrb`
2. **Bytecode → C Header:** `arraynize.rb` converts `.mrb` to `src/mrb_data.h`
3. **C → NES ROM:** cc65 toolchain produces final `.nes` file

### Memory Configuration

Critical limits in `src/vm_config.h`:

```c
#define MRBC_MEMORY_SIZE 724     // Total VM memory pool
#define MAX_SYMBOLS_COUNT 40     // Maximum user symbols  
#define MAX_REGS 40             // Maximum VM registers
```

You can edit them for your game script but if too big it will cause runtime memory corruption.

## Testing Framework

### Test Structure

Tests are located in `test/nones/` with this structure:
- `*.rb` - Ruby source files
- `*.mrb` - Compiled bytecode (generated)
- `*.expected_out` - Expected program output

### Running Tests

```bash
# Run full test suite with AddressSanitizer
rake :nones_test

# Run individual test
./nones_test test/nones/example.mrb
```

### Adding New Tests

1. Create `test/nones/your_test.rb`
2. Add expected output to `test/nones/your_test.expected_out`
3. Run tests to verify

## Contributing

### Setting Up Development Environment

1. **Clone and build:**
   ```bash
   git clone https://github.com/yhara/nesruby.git
   cd nesruby
   rake
   ```

2. **Verify test environment:**
   ```bash
   rake :nones_test
   ```

3. **Install development tools:**
   - NES emulator (Nestopia, FCEUX, etc.)
   - Debugger for cc65 (optional)
   - AddressSanitizer-capable compiler

### Development Workflow

1. **Make changes** to VM, runtime, or build system
2. **Test locally** with `rake :nones_test`
3. **Build NES ROM** with `rake`
4. **Test on emulator** to verify NES compatibility
5. **Submit pull request** with test coverage

### NES-Specific Considerations

**Hardware Constraints:**
- 2KB RAM total (shared with stack and variables)
- 6502 CPU with specific instruction timing
- No floating-point hardware
- Limited addressing modes

**Development Tips:**
- Test frequently on real hardware or accurate emulators
- Profile for timing-sensitive operations
- Consider battery-backed save data limitations
- Account for different NES variants (NTSC/PAL)

## Debugging

### VM Debugging

**AddressSanitizer (nones_test):**
```bash
# Compile with debugging symbols
CFLAGS="-g -O0" rake :nones_test

# Run with verbose output
./nones_test -v test/nones/your_test.mrb
```

**NES Debugging:**
- Use emulator debuggers (FCEUX, Mesen)
- Add debug output via NES graphics

## Resources

- [mruby/c Documentation](https://github.com/mrubyc/mrubyc)
- [cc65 Documentation](https://cc65.github.io/)
- [NES Development Wiki](https://www.nesdev.org/)
- [6502 Assembly Reference](http://www.6502.org/)

For questions, bug reports, or contributions, please use the [GitHub Issues](https://github.com/yhara/nesruby/issues) page.
