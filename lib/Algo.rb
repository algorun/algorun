=begin
    Algorun turns command-line algorithms into ready-to-use web enabled 
    containers - Copyright (C) 2015 Thibauld Favre <tfavre@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
=end

class Algo
	
	def initialize(input)
		@input_data = input
	end

	def run()
		Dir.chdir ENV["CODE_HOME"] + "src/"
		@input_file = "input.txt"
		fw = File.open(@input_file, "w")
		fw.write(@input_data)
		fw.close()
		
		@command = ''
		if not ENV["EXEC_COMMAND"].nil? then
			@command = ENV["EXEC_COMMAND"]
			@command = @command.strip
		end
		@command_options = ''
		if not ENV["COMMAND_OPTIONS"].nil? then
			@command_options = ENV["COMMAND_OPTIONS"]
			@command_options = " " + @command_options.strip
		end
		if not ENV["OUTPUT_FILE_NAME"].nil? then
			@output_file = ENV["OUTPUT_FILE_NAME"]
			@output_file = @output_file.strip
			system(@command + " " + @input_file + @command_options)
		else
			@output_file = "output.txt"
			system(@command + " " + @input_file + @command_options + " -f " + @output_file)		
		end
		
		@result = ''
		fr = File.open(@output_file, "r")
		fr.each_line do |line|
			@result += line
		end

		fr.close()
		
		return @result
	end
    
    def mytest()
        return @input_data + ' added for test'
    end
end


if $0 == __FILE__ then
	input = ARGV[0].dup
    task = Algo.new(input)
    puts task.run()
end