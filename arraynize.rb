mrb = ARGF.read
puts "// Generated by arraynize.rb (#{mrb.bytesize} bytes)"
puts "const unsigned char mrb_data[]={"
puts mrb.bytes.map{_1.to_s}.join(",")
puts "};\n"
