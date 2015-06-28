#!/usr/bin/ruby
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

require_relative './lib/Task.rb'
require_relative './lib/Algo.rb'
require_relative './lib/Server.rb'

if ENV["CODE_HOME"].nil? then
	ENV["CODE_HOME"]=ENV["PWD"]
end

server = WEBrick::HTTPServer.new(:Port => 8765, "RequestTimeout" => 300, :DocumentRoot => ENV["CODE_HOME"]+"web/")
server.mount("/do", Algorun)
trap("INT") {
	server.shutdown
}
server.start()
