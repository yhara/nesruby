rule ".mrb" => ".rb" do |t|
  sh "mrbc -v -o #{t.name} #{t.source}"
end

task arraynize: ["arraynize.rb", "a.mrb"] do
  sh "ruby arraynize.rb a.mrb"
end

task :default do
  sh "make"
  sh "cp bin/hello.nes /mnt/c/work/nes01/"
end
