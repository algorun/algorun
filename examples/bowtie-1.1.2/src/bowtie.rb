require 'open3'

def number_or_nil(string)
  Integer(string || '')
rescue ArgumentError
  nil
end

input_file = ARGV[0].dup
fr = File.open(input_file, "r")
input_data = ""
fr.each_line do |line|
	input_data += line	
end
fr.close()
input_data.strip!

# execute algorithm here
options = ""

if number_or_nil(ENV["Skip"]).nil? then
	options += ""
else
	options += " -s " + ENV["Skip"].strip
end

if number_or_nil(ENV["Only-Align"]).nil? then
	options += ""
else
	options += " -u " + ENV["Only-Align"].strip
end

if number_or_nil(ENV["Trim-Left"]).nil? then
	options += ""
elsif number_or_nil(ENV["Trim-Left"]) != 0
	options += " -5 " + ENV["Trim-Left"].strip
end

if number_or_nil(ENV["Trim-Right"]).nil? then
	options += ""
elsif number_or_nil(ENV["Trim-Right"]) != 0
	options += " -3 " + ENV["Trim-Right"].strip
end

if number_or_nil(ENV["Phred-Quality"]) == 64 then
	options += " --phred64-quals"
end

if ENV["Solexa"] == "on" then
	options += " --solexa-quals"
end

# because -v and -n are mutually exlusive
if number_or_nil(ENV["Align-v"]).nil? then
	if number_or_nil(ENV["Align-n"]) != 2 then
		options += " -n " + ENV["Align-n"].strip
	end
elsif number_or_nil(ENV["Align-v"]) != 0 then
	options += " -v " + ENV["Align-v"].strip
elsif number_or_nil(ENV["Align-n"]) != 2 then
	options += " -n " + ENV["Align-n"].strip
end

if number_or_nil(ENV["Align-e"]).nil? then
	options += ""
elsif number_or_nil(ENV["Align-e"]) != 70
	options += " -e " + ENV["Align-e"].strip
end

if number_or_nil(ENV["Align-l"]).nil? then
	options += ""
elsif number_or_nil(ENV["Align-l"]) != 28
	options += " -l " + ENV["Align-l"].strip
end

if number_or_nil(ENV["Align-I"]).nil? then
	options += ""
elsif number_or_nil(ENV["Align-I"]) != 0
	options += " -I " + ENV["Align-I"].strip
end

if number_or_nil(ENV["Align-X"]).nil? then
	options += ""
elsif number_or_nil(ENV["Align-X"]) != 250
	options += " -X " + ENV["Align-X"].strip
end

if number_or_nil(ENV["Report-k"]).nil? then
	options += ""
elsif number_or_nil(ENV["Report-k"]) != 1
	options += " -k " + ENV["Report-k"].strip
end

if ENV["Report-all"] == "on" then
	options += " --all"
end

if number_or_nil(ENV["Report-m"]).nil? then
	options += ""
else
	options += " -m " + ENV["Report-m"].strip
end

if ENV["Report-best"] == "on" then
	options += " --best"
end

if ENV["Report-strata"] == "on" then
	options += " --strata"
end

if ENV["suppress"] != "0" then
	options += " --suppress " + ENV["suppress"].delete(' ')
end

options.strip!
command = "./bowtie " + options + " e_coli -c " + input_data
stdin, stdout, stderr, wait_thr = Open3.popen3(command)

# write results to a file
fw = File.open("output.txt", "w")
fw.write(stdout.read)
fw.write(stderr.read)
fw.close()
