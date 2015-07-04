#!/usr/bin/env ruby

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

require 'webrick'
require 'open-uri'
require 'json'
require 'net/http'

=begin
    Example usage: 
        http://localhost:1234/do/run [POST with an input=<json> param]
=end

class Algorun < WEBrick::HTTPServlet::AbstractServlet

    def do_POST (request, response)
	output=""
	case request.path
		when "/do/run"
			response.status = 500
			input = request.query["input"]
			if not ENV["VALIDATE_INPUT"].nil? then
				eval("def validate_input(json)\n"+ENV["VALIDATE_INPUT"]+"\nend\n")
			end
			check_input=self.respond_to?(:validate_input)
			if ((not check_input) || (check_input && validate_input(input))) then
				begin
					task = Algo.new(input)
					output = task.run()
					if not ENV["OUTPUT_TO"].nil? then
						output_to=eval(ENV["OUTPUT_TO"])
						output_to.each do |node|
							eval("def check(json)\n"+node["condition"]+"\nend\n")
							if check(output) then
								Net::HTTP.post_form(URI(node["target"]),'input'=>output)
								puts output
							end
						end
					end
					#response.content_type = "application/json"
					response.body = output+ "\n"
				rescue StandardError=>e
					output = "ERROR: " + e.to_s
				end
				response.status = 200
			else
				output = "PASS: invalid input"
			end
		when "/do/config"
			request.query.each do |param_name, param_value|
				if not ENV[param_name].nil? then
					ENV[param_name] = param_value
					output = "Parameter " + param_name + " changed to " + param_value
				else
					output = "Cannot find this parameter in environment variables"
				end
			end
		else
			output += "failure"
			response.status = 404
	end
	#response.content_type = "application/json"
	response.body = output + "\n"
    end
end

if $0 == __FILE__ then
	server = WEBrick::HTTPServer.new(:Port => 80, "RequestTimeout" => 3000, :DocumentRoot => "web/")
	server.mount "/", Algorun
	trap("INT") {
		server.shutdown
	}
	server.start
end
