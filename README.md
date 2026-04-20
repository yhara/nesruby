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

1. `vi game.rb`
1. `rake`
1. Open bin/hello.nes with an NES emulator (or NES itself, of cource, if you can)

## Guides

### API

See `./gen_builtin_symbols.rb` for the list of APIs.

The behavior is hard-coded in `op_ssend` in `src/vm.c`.

### Graphic

#### Sprite and Font

Included in tileset.chr. The font part is copied from Shiru's tutorial.

.chr can be edited using tools like [YYCHR](https://w.atwiki.jp/yychr/)(Windows).

#### Background

I've not tried background drawing yet

### Sound

`./sounds.s` is copied from Shiru's tutorial.

### Music

How to make ./music.s

1. Make music.ftm using [FamiTracker](http://famitracker.com) (tested with 0.4.6)
2. Convert music.ftm to music.s using [famitone2](https://shiru.untergrund.net/articles/programming_nes_games_in_c.htm)/tools/text2data.exe
  - `Instrument 0 does not have volume envelope`: Edit that instrument in FamiTracker and give it some volume envelope change
  - `Parsing error: SequenceTypeCount not found`: shown when I use dn-famitracker
3. Copy the generated .s as music.s

## Acknowledgements

Huge thanks to http://shiru.untergrund.net/articles/programming_nes_games_in_c.htm
  (via https://github.com/jmk/cc65-nes-examples/ )
and mruby/c

## Lisence

MIT
