# nesruby

.mrb interpreter for NES

## Example

https://yhara.github.io/nesruby/

## Project status

- [x] nil, true, false, Integer, Symbol 
- [ ] String, Array, etc.
- [ ] GC
- Maybe
    - Exception handling

## Prerequisites

- [cc65](https://cc65.github.io/), make
- mruby (for mrbc)
- ruby (for tooling)

## How to run

1. `vi a.rb`
1. `rake`
1. Open bin/hello.nes with an NES emulator (or NES itself, of cource, if you can)

## Acknowledgements

- Huge thanks to http://shiru.untergrund.net/articles/programming_nes_games_in_c.htm
  (via https://github.com/jmk/cc65-nes-examples/ )

## Lisence

MIT
