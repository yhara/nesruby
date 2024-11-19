COMPILER = "cc65"
ASSEMBLER = "ca65"
LINKER = "ld65"
ASFLAGS = ""
CFLAGS = "-t nes -I./include -Oi --add-source"
LDFLAGS = "-C nrom_128_horz.cfg"

MRB_DATA = "src/mrb_data.h"
file "a.mrb" => "a.rb" do |t|
  sh "mrbc -v -o #{t.name} #{t.source}"
end
file MRB_DATA => ["a.mrb", "arraynize.rb"] do |t|
  sh "ruby arraynize.rb #{t.source} a.rb > #{t.name}"
end

file "obj/crt0.o" => "crt0.s" do |t|
  sh "#{ASSEMBLER} #{ASFLAGS} -o #{t.name} #{t.source}"
end

SRCS = Dir["src/*.c"] + ["main.c"]
OBJS = SRCS.map { |s| "obj/#{File.basename s, ".c"}.o" }
SRCS.zip(OBJS).each do |src, obj|
  file obj => [src, MRB_DATA] do |t|
    asm = t.name.sub(/\.o$/, ".s")
    sh "#{COMPILER} #{CFLAGS} -o #{asm} #{t.source}"
    sh "#{ASSEMBLER} #{ASFLAGS} -o #{t.name} #{asm}"
  end
end

file "bin/nesruby.nes" => ["obj/crt0.o", *OBJS, MRB_DATA] do |t|
  sources = t.sources - [MRB_DATA]
  sh "#{LINKER} #{LDFLAGS} -o #{t.name} #{sources.join " "} nes.lib"
end

task :clean do
  rm Dir["obj/*.[os]"]
  rm "a.mrb"
  rm MRB_DATA
end

task default: "bin/nesruby.nes"
