NAME = "game"
MRBC = if RUBY_PLATFORM =~ /darwin/
         # mruby 3.3.0 is broken on intel mac
         "~/research/mruby/bin/mrbc"
       else
         "mrbc"
       end
MRUBY_STRIP = "mruby-strip"
CFG = "nrom_256_horz.cfg"
COMPILER = "cc65"
ASSEMBLER = "ca65"
LINKER = "ld65"
ASFLAGS = ""
CFLAGS = "-t nes -I./include -Oi --add-source"
LDFLAGS = "-C #{CFG}"

MRB_DATA = "src/mrb_data.h"
file "#{NAME}.mrb" => "#{NAME}.rb" do |t|
  sh "#{MRBC} -v -o #{t.name} #{t.source}"
  sh "#{MRUBY_STRIP} -l #{t.name}"
end
file MRB_DATA => ["#{NAME}.mrb", "arraynize.rb"] do |t|
  sh "ruby arraynize.rb #{t.source} #{NAME}.rb > #{t.name}"
end

BUILTIN_SYMBOLS = "src/_autogen_builtin_symbol.h"
file BUILTIN_SYMBOLS => "gen_builtin_symbols.rb" do |t|
  sh "ruby gen_builtin_symbols.rb > #{t.name}"
end

file "obj/crt0.o" => ["crt0.s", "music.s", "sounds.s", "tileset.chr", "neslib.s"] do |t|
  sh "#{ASSEMBLER} #{ASFLAGS} -o #{t.name} #{t.source}"
end

INCLUDES = Dir["src/*.h"] + Dir["include/*.h"]
DEPS = [*INCLUDES, MRB_DATA, BUILTIN_SYMBOLS]

SRCS = Dir["src/*.c"] + ["main.c"]
OBJS = SRCS.map { |s| "obj/#{File.basename s, ".c"}.o" }
SRCS.zip(OBJS).each do |src, obj|
  file obj => [src, *DEPS] do |t|
    asm = t.name.sub(/\.o$/, ".s")
    sh "#{COMPILER} #{CFLAGS} -o #{asm} #{t.source}"
    sh "#{ASSEMBLER} #{ASFLAGS} -o #{t.name} #{asm}"
  end
end

file "bin/nesruby.nes" => ["obj/crt0.o", *OBJS, MRB_DATA, CFG] do |t|
  sources = t.sources - [MRB_DATA, CFG]
  sh "#{LINKER} #{LDFLAGS} -o #{t.name} #{sources.join " "} nes.lib"
end

task :clean do
  rm Dir["obj/*.[os]"]
  rm "#{NAME}.mrb"
  rm MRB_DATA
end

task default: "bin/nesruby.nes"
