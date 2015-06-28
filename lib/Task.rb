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

class TaskMethod
	attr_reader :type, :id, :description, :arguments
	def initialize(method)
		@type=method['type']
		@id=method['id']
		@description=method['desription']
		@arguments={}
		if !method['arguments'].empty? then
			method['arguments'].each do |arg|
				@arguments[arg['name']]=arg['value']
			end
		end
	end
end

class Model
	attr_reader :fieldCardinality, :type, :description, :name, :version, :parameters, :updateRules, :variables, :variableScores, :simlab
	def initialize(model)
		@type=model['type']
		@description=model['description']
		@name=model['name']
		@version=model['version']
		@fieldCardinality=model['fieldCardinality']
		@parameters=model['parameters']
		@updateRules=model['updateRules']
		@variables=model['variables']
		@variableScores=model['variableScores']
		@simlab=model['simlab']
	end
end

class TimeSeries
	attr_reader :data
	def initialize(ts)
		@data=ts['timeSeriesData']
	end

	def get_numberOfVariables()
		if !@data.empty? then
			return @data[0]['matrix'][0].length
		end
		return 0
	end

	def get_wildType()
		wt=[]
		@data.each do |tdata|
			if tdata['index'].empty? then
				wt.push(tdata['matrix'])
			end
		end
		return wt
	end

	def get_knockOut()
		ko=[]
		@data.each do |tdata|
			if !tdata['index'].empty? then
				ko.push([tdata['index'][0],tdata['matrix']])
			end
		end
		return ko
	end
end

class Task
	def initialize(json,exec_file)
		@raw_output=nil
		@output=nil
		@numberOfVariables=nil
		@exec_file=exec_file
		@errors=[]
		@warnings=[]
		@task = json['task']
		if @task.nil? then  raise "JSON_FILE_HAS_NO_TASK" end
		if @task['input'].nil? then  raise "TASK_HAS_NO_INPUT" end
		if @task['method'].nil? then  raise "TASK_HAS_NO_METHOD" end
		@method=TaskMethod.new(@task['method'])
		@input = {
			"models"=>{},
			"timeSeries"=>[],
			"custom"=>[]
		}
		@task['input'].each do |o|
			case o['type'].downcase
			when "model"
				@input['models'][o['name']]=Model.new(o)
			when "timeseries"
				@input['timeSeries'].push(TimeSeries.new(o))
			else
				@input['custom'].push(input)
			end
		end
		if !@input['timeSeries'].empty? then
			@numberOfVariables=@input['timeSeries'][0].get_numberOfVariables()
		end
		@tmp_files=[]
		self.validate_input()
		if !@warnings.empty? then @warnings.each {|w| STDERR.puts(w.to_s) } end
		if !@errors.empty? then
			@errors.each {|e| STDERR.puts(e.to_s) } 
			raise "%i INPUT ERRORS FOUND AND %i WARNINGS ISSUED" % [@errors.length, @warnings.length]
		end
	end

	def validate_input
		@errors.push(:INPUT_VALIDATION_NOT_IMPLEMENTED)
	end

	def render_output()
		puts "RENDER_OUTPUT_METHOD_NOT_IMPLEMENTED";
	end

	def clean_temp_files()
		@tmp_files.each { |f| File.delete(f) }
	end
end
