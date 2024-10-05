puts "const unsigned char mrb_data[]={"
puts ARGF.read.bytes.map{_1.to_s}.join(",")
puts "};\n"
