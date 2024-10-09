rule ".mrb" => ".rb" do |t|
  sh "mrbc -v -o #{t.name} #{t.source}"
end

file "src/mrb_data.h" => ["arraynize.rb", "a.mrb"] do
  sh "ruby arraynize.rb a.mrb > src/mrb_data.h"
end

file "bin/nesruby.nes" => ["src/mrb_data.h", "src/main.c"] do
  sh "make"
end

task default: "bin/nesruby.nes" do
  sh "cp bin/nesruby.nes /mnt/c/work/nes01/"
end
