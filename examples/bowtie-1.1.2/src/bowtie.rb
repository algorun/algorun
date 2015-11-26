require 'open3'

# helper function to check numerical values
def number_or_nil(string)
  Integer(string || '')
rescue ArgumentError
  nil
end

# read input data that is passed directly
input_data=ARGV[0].strip

# form the options string by reading envirnment variables
options = ""

options += " -s " + ENV["Skip"].strip unless number_or_nil(ENV["Skip"]).nil?
options += " -u " + ENV["Only-Align"].strip unless number_or_nil(ENV["Only-Align"]).nil?
options += " -5 " + ENV["Trim-Left"].strip unless number_or_nil(ENV["Trim-Left"]).nil? or number_or_nil(ENV["Trim-Left"]) == 0
options += " -3 " + ENV["Trim-Right"].strip unless number_or_nil(ENV["Trim-Right"]).nil? or number_or_nil(ENV["Trim-Right"]) == 0
options += " --phred64-quals" if number_or_nil(ENV["Phred-Quality"]) == 64
options += " --solexa-quals" if ENV["Solexa"] == "on"
options += " -n " + ENV["Align-n"].strip if number_or_nil(ENV["Align-v"]).nil? and number_or_nil(ENV["Align-n"]) != 2 
options += " -v " + ENV["Align-v"].strip unless number_or_nil(ENV["Align-v"]).nil? or number_or_nil(ENV["Align-v"]) == 0
options += " -n " + ENV["Align-n"].strip unless number_or_nil(ENV["Align-v"]).nil? or number_or_nil(ENV["Align-n"]) == 2
options += " -e " + ENV["Align-e"].strip unless number_or_nil(ENV["Align-e"]).nil? or number_or_nil(ENV["Align-e"]) == 70
options += " -l " + ENV["Align-l"].strip unless number_or_nil(ENV["Align-l"]).nil? or number_or_nil(ENV["Align-l"]) == 28
options += " -I " + ENV["Align-I"].strip unless number_or_nil(ENV["Align-I"]).nil? or number_or_nil(ENV["Align-I"]) == 0
options += " -X " + ENV["Align-X"].strip unless number_or_nil(ENV["Align-X"]).nil? or number_or_nil(ENV["Align-X"]) == 250
options += " -k " + ENV["Report-k"].strip unless number_or_nil(ENV["Report-k"]).nil? or number_or_nil(ENV["Report-k"]) == 1
options += " --all" if ENV["Report-all"] == "on"
options += " -m " + ENV["Report-m"].strip unless number_or_nil(ENV["Report-m"]).nil?
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
