input_file = ARGV[0].dup
fr = File.open(input_file, "r")
fw = File.open("output.txt", "w")
fr.each_line do |line|
	fw.write(line + " added for test")
end
fr.close()
fw.close()
