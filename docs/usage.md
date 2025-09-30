---
layout: page
title: Usage
permalink: /usage/
---

# Usage Guide

## Basic Workflow

It is highly recommended to start with modifing the example. Build and save often (eg. `git commit`) so that you can rollback easily when something went wrong.

## Installation

See [Installation]({% link installation.md %}) for how to build example `.nes`.

## Basic knowledge

It would be helpful to know the development process of NES games in C.

https://www.nesdev.org/

## How to edit the Ruby program

Edit `a.rb`.

## How to edit C libfuncs

Edit `src/libfunc.c`.

By default, the following methods are defined.

- `wait_frame()`
- `draw_arrow(x, y)` draws the "arrow" character at (x, y).
- `draw_ruby(x, y)` draws the "ruby" character at (x, y).
- `pad_trigger(n)` returns the state of the `n`-th pad.
- `btn_a_pressed(state)` returns true if "A" button is pressed.
- `show_title()` prints the game title. (message is hardcoded in libfunc.c)
- `hide_title()` hides the game title.
- `rand8()` returns an integer between 0 to 255.
- `play_music(n)` plays the `n`-th music.
- `stop_music()` stops the music.
- `play_sound(n, channel)` plays the `n`-th sound.
- `dbg(n)` prints the given integer `n`.

To add a new method, edit `gen_builtin_symbols.rb`.

### Underlying layer

These builtin functions are implemented using helper library provided by
 [`shiru.untergrund.net`](https://shiru.untergrund.net/articles/programming_nes_games_in_c.htm).

Functions like `ppu_wait_frame`, `oam_spr` or `music_play` is defined in include/neslib.h and implemented in `neslib.s`. Audio support is implemented in `famitone2.s`. Startup code is in `crt0.s`.

## Editing music

Music is defined in `music.s` and included by `crt0.s`.

TBA: how to edit (I made it by famitracker and converted with some tool)

## Editing sfx

Sfx are defined in `sounds.s` and included by `crt0.s`.

TBA: how to edit (this file is included in Shiru's sample and I don't know how to edit)

## Editing character

Characters are defined in `tileset.chr`. This file can be edited with softwares like

- [YY-CHR (Windows)](https://w.atwiki.jp/yychr/#google_vignette)

## Editing font

Font is also in the file `tileset.chr` and can be edited in the same way.
