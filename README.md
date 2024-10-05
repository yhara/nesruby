# nesruby

.mrb interpreter for NES

## Project status

- Just started
- Can load an integer (OP_LOADI) in .mrb and show it

## Prerequisites

- [cc65](https://cc65.github.io/), make
- mruby (for mrbc)
- ruby (for tooling)

## How to run

1. `vi a.rb`
2. `mrb a.rb` # generates a.mrb
3. `ruby arraynize.rb a.rb`
4. Copy and paste the output into src/main.c
5. `make`
6. Open bin/hello.nes with an NES emulator (or NES itself, of cource, if you can)

## Acknowledgements

- `Makefile` and `nes.cfg` is taken from http://muto.world.coocan.jp/nesapp.html . Huge thanks

## Lisence

MIT
