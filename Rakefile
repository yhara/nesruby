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
file "a.mrb" => "a.rb" do |t|
  sh "#{MRBC} -v -o #{t.name} #{t.source}"
  sh "#{MRUBY_STRIP} -l #{t.name}"
end
file MRB_DATA => ["a.mrb", "arraynize.rb"] do |t|
  sh "ruby arraynize.rb #{t.source} a.rb > #{t.name}"
end

BUILTIN_SYMBOLS = "src/_autogen_builtin_symbol.h"
file BUILTIN_SYMBOLS => "gen_builtin_symbols.rb" do |t|
  sh "ruby gen_builtin_symbols.rb > #{t.name}"
end

file "obj/crt0.o" => "crt0.s" do |t|
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
  rm "a.mrb"
  rm MRB_DATA
end

task default: "bin/nesruby.nes"

#
# No-NES test
#
CLANG = "clang-17"
CLANG_OPTS = "-g -fsanitize=address"
task :nones do
  deps = %w[symbol load vm libfunc debug value]
  deps.each do |name|
    sh "#{CLANG} #{CLANG_OPTS} -c src/#{name}.c -o obj/#{name}.o"
  end
  #sh "#{cc} nones_test.c #{opts} -o a.out #{deps.map{ "obj/#{_1}.o" }.join(' ')}"
end

NONES_TEST_RBS = Dir["test/nones/*.rb"]
NONES_TEST_RBS.each do |rb_path|
  mrb_path = rb_path.sub(".rb", ".mrb")
  file mrb_path => rb_path do
    sh "#{MRBC} -o #{mrb_path} #{rb_path}"
  end
end
NONES_TEST_MRBS = NONES_TEST_RBS.map{ _1.sub(".rb", ".mrb") }

task :nones_test => [:nones, *NONES_TEST_MRBS] do
  deps = %w[symbol load vm libfunc debug value]
  bin = "_nones_test.out"
  sh "#{CLANG} nones_test.c #{CLANG_OPTS} -o #{bin} #{deps.map{ "obj/#{_1}.o" }.join(' ')}"
  NONES_TEST_MRBS.each do |mrb_path|
    expected_out = mrb_path.sub(".mrb", ".expected_out")
    File.write(expected_out, "todo") if !File.exist?(expected_out)
    actual_out = mrb_path.sub(".mrb", ".actual_out")
    sh "./#{bin} #{mrb_path} > #{actual_out}"
    sh "diff #{expected_out} #{actual_out}"
  end
end
