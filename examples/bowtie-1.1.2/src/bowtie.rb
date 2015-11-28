require 'open3'

# read input data that is passed directly
input_data=ARGV[0].strip

# form the options string by reading envirnment variables
options = ""

options += " -s " + ENV["Skip"].strip
options += " -u " + ENV["Only-Align"].strip unless ENV["Only-Align"] == "all"
options += " -5 " + ENV["Trim-Left"].strip
options += " -3 " + ENV["Trim-Right"].strip
options += " --phred64-quals" if ENV["Phred-Quality"] == "64"
options += " --solexa-quals" if ENV["Solexa"] == "on"
options += " -n " + ENV["Align-n"].strip
options += " -v " + ENV["Align-v"].strip
options += " -n " + ENV["Align-n"].strip
options += " -e " + ENV["Align-e"].strip
options += " -l " + ENV["Align-l"].strip
options += " -I " + ENV["Align-I"].strip
options += " -X " + ENV["Align-X"].strip
options += " -k " + ENV["Report-k"].strip
options += " --all" if ENV["Report-all"] == "on"
options += " -m " + ENV["Report-m"].strip unless ENV["Report-m"] == "no-limit"
options += " --best" if ENV["Report-best"] == "on"
options += " --strata" if ENV["Report-strata"] == "on"
options += " --suppress " + ENV["suppress"].delete(' ') unless ENV["suppress"] == "0"
options.strip!

# run the algorithm with the options injected
command = "./bowtie " + options + " e_coli -c " + input_data
stdin, stdout, stderr, wait_thr = Open3.popen3(command)

# print the output to the standard output stream
puts stdout.read
puts stderr.read
