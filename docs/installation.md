---
layout: page
title: Installation
permalink: /installation/
---

# Installation Guide

## Prerequisites

nesruby requires several tools and libraries to build and run:

### Required Dependencies

1. **cc65** - C compiler/assembler/linker for NES development
2. **mruby** - Ruby implementation providing the `mrbc` compiler
  - tested with mruby 3.3.0
3. **Ruby** - Required for build scripts
4. **Rake** - Build system (usually comes with Ruby)

## Platform-Specific Installation

### macOS

```bash
# Install cc65 via Homebrew
brew install cc65

# Install mruby (may require specific version for compatibility)
brew install mruby

# Verify Ruby and Rake are available
ruby --version
rake --version
```

### Linux (Ubuntu/Debian)

```bash
# Install cc65
sudo apt-get update
sudo apt-get install cc65

# Install mruby
sudo apt-get install mruby mruby-dev

# Install Ruby and Rake if not present
sudo apt-get install ruby ruby-dev rake

# Verify installations
cc65 --version
mrbc --version
ruby --version
```

### Linux (Other Distributions)

For other Linux distributions, install equivalent packages:
- cc65 (may be in repositories or build from source)
- mruby and mruby-dev
- ruby and rake

### Windows

Windows support requires setting up a Unix-like environment:

1. **Install WSL2** (recommended) and follow Linux instructions
2. **Or use MSYS2/MinGW** with equivalent packages

## Installing nesruby

### From Source

1. **Clone the repository:**
   ```bash
   git clone https://github.com/yhara/nesruby.git
   cd nesruby
   ```

2. **Verify dependencies:**
   ```bash
   # Check that all tools are available
   cc65 --version
   mrbc --version
   ruby --version
   rake --version
   ```

3. **Build nesruby:**
   ```bash
   # Build the main NES ROM
   rake

   # This should create bin/nesruby.nes
   ls -la bin/nesruby.nes
   ```

4. **Run tests (optional):**
   ```bash
   # Test the VM without NES dependencies
   rake :nones_test
   ```

## Verification

After installation, verify everything works:

1. Edit `./a.rb`
2. Compile and build:
   ```bash
   rake
   ```
3. Test with NES emulator:
   Use an NES emulator like [Mesen](https://www.mesen.ca/), [FCEUX](https://fceux.com/web/home.html), or [JSNES](https://jsnes.org/) to run `bin/nesruby.nes`

## Troubleshooting

### Common Issues

**"mrbc: command not found"**
- Ensure mruby is properly installed and in your PATH
- On intel mac, you may need to use mruby >= 3.4.0

**"cc65: command not found"**
- Install cc65 development tools for your platform
- Verify cc65 is in your system PATH

**Build fails with memory errors**
- Check that your Ruby code fits within nesruby's memory constraints
- Current limits: 724 bytes total memory, 40 symbols max

**Bytecode compatibility issues**
- Ensure you're using a compatible mruby version
- Different mruby versions may generate incompatible bytecode

For additional help, please check the [GitHub Issues](https://github.com/yhara/nesruby/issues) or create a new issue.
