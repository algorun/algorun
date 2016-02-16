require 'json'
require 'matrix'
require 'pp'
require_relative 'Task.rb'
require_relative 'React.rb'
require_relative 'Server.rb'

class React < Task
	def initialize(json,exec_file)
		@file_manager=nil
		@arguments={}
		@default_values= {
			'HammingPolyWeight'=>ENV['HammingPolyWeight'],
			'ComplexityWeight'=>ENV['ComplexityWeight'],
			'RevEngWeight'=>ENV['RevEngWeight'],
			'BioProbWeight'=>ENV['BioProbWeight'],
			'HammingModelWeight'=>ENV['HammingModelWeight'],
			'PolyScoreWeight'=>ENV['PolyScoreWeight'],
			'GenePoolSize'=>ENV['GenePoolSize'],
			'NumCandidates'=>ENV['NumCandidates'],
			'NumParentsToPreserve'=>ENV['NumParentsToPreserve'],
			'MaxGenerations'=>ENV['MaxGenerations'],
			'StableGenerationLimit'=>ENV['StableGenerationLimit'],
			'MutateProbability'=>ENV['MutateProbability']
		}
		super(json,exec_file)
	end

	def validate_input
		# input must contain a timeseries
		if @input['timeSeries'].empty? then @errors.push(:TIMESERIES_REQUIRED) end
		# input should contain only 1 timeseries
		if @input['timeSeries'].length>1 then @warnings.push(:ONLY_FIRST_TIMESERIES_WILL_BE_TAKEN_INTO_ACCOUNT) end
		# timeseries must contain only binary matrixes
		@input['timeSeries'].each do |ts|
			ts.data.each do |m|
				m['matrix'].each do |r|
					if r.max>1 then @errors.push(:TIMESERIES_MATRIXES_MUST_BE_BINARY) end
				end
				if !m['index'].empty? then
					if m['index'].length>1 then @warnings.push(:ONLY_FIRST_NODE_WILL_BE_KNOCKED_OUT) end
				end
			end
		end
		# timeseries must contain at least 1 wildtype
		wt_found=false
		@input['timeSeries'].each do |ts|
			ts.data.each do |m|
				if m['index'].empty? then wt_found=true end
			end
		end
		if !wt_found then @errors.push(:TIMESERIES_MUST_CONTAIN_AT_LEAST_1_WILDTYPE) end
		# if a model is present, its fieldcardinity must be equal to 2
		if !@input['polynomialDynamicalSystemSet'].empty? then
			@input['polynomialDynamicalSystemSet'].each do |m|
				if m.fieldCardinality!=2 then @errors.push(:MODELS_FIELD_CARDINALITY_MUST_BE_2) end
			end
		end

		# the following arguments are required
		required=[
			'HammingPolyWeight',
			'ComplexityWeight',
			'RevEngWeight',
			'BioProbWeight',
			'HammingModelWeight',
			'PolyScoreWeight',
			'GenePoolSize',
			'NumCandidates',
			'NumParentsToPreserve',
			'MaxGenerations',
			'StableGenerationLimit',
			'MutateProbability'
		]
		required.each do |arg|
			# if @method.arguments[arg].nil? then @errors.push(("MISSING_ARGUMENT_ERROR_"+arg.upcase).to_sym) end
			if @arguments[arg].nil? then
				if not @default_values[arg].nil? then
					@arguments[arg]=@default_values[arg]
				end
			end
		end
	end

	def get_fieldCardinality()
		return "P=2;\n"	
	end

	def get_numberOfVariables()
		return "N=%i;\n" % @numberOfVariables
	end

	def get_wildType()
		res="WT={"
		wt=@input['timeSeries'][0].get_wildType()
		wt_array=[]
		wt.each_with_index do |w,i|
			tmp=""
			w.each { |r| tmp+=r.join(' ')+"\n" }
			File.open("w%i.txt" % i,'w') { |f| f.write(tmp) }
			wt_array.push("\"w%i.txt\"" % i)
			@tmp_files.push("w%i.txt" % i)
		end
		res+=wt_array.join(',').to_s
		res+="};\n"
		return res
	end

	def get_knockOut()
		res="KO={"
		ko=@input['timeSeries'][0].get_knockOut()
		ko_array=[]
		ko.length.times do |i|
			tmp=""
			ko[i][1].each { |r| tmp+=r.join(' ')+"\n" }
			File.open("K%i.txt" % i,'w') { |f| f.write(tmp) }
			ko_array.push("(%i,\"K%i.txt\")" % [ko[i][0], i])
			@tmp_files.push("K%i.txt" % i)
		end
		res+=ko_array.join(',').to_s
		res+="};\n"
		return res
	end

	def get_priorReverseEngineeringNetwork()
		model=@input['priorReverseEngineeringNetwork']
		if !model.empty? then
			res=""
			model.variableScores.each_with_index do |vs,i|
				i_baby=i+1
				res+="F%i " % i_baby
				row=Array.new(@numberOfVariables,0)
				vs['sources'].each do |s|
					idx=s['source'].slice!("x")
					row[idx.to_i]=s['score'].to_f.to_s
				end
				res+=row.join(' ')+" 1\n"	
			end
			File.open('RevMat.txt','w') { |f| f.write(res) }
			@tmp_files.push("RevMat.txt")
			return "REV={\"RevMat.txt\"};\n"
		else
			return "REV={};\n"
		end
	end

	def get_complexity()
		# not supported by REACT at this time
		return "CMPLX={};\n"
	end

	def get_priorBiologicalNetwork()
		if !@arguments['priorBiologicalNetwork'].nil? then
			res=""
			@arguments['priorBiologicalNetwork'].each_with_index do |r,i|
				idx=i+1
				res+="F%i " % idx
				res+=r.join(" ")+"\n"
			end
			File.open("Bio.txt",'w') { |f| f.write(res) }
			@tmp_files.push("Bio.txt")
			return "BIO={\"Bio.txt\"};\n"
		else
			return "BIO={};\n"
		end
	end

	def get_priorModel()
		res="MODEL={"
		model=@input['polynomialDynamicalSystemSet'][0]
		if !model.nil? then
			functions=[]
			combinations=1
			model.updateRules.each do |rule|
				var=[]
				rule['functions'].each { |f| var.push(f['polynomialFunction']) }
				functions.push(var)
				combinations*=var.length
			end
			mx=[]
			functions.each do |f|
				mx.push(f*(combinations/f.length))
			end
			models=Matrix.rows(mx).t().to_a()
			models.each_with_index do |m,i|
				tmp=""
				m.each_with_index { |f,j| tmp+="f%i=%s\n" % [j+1,f] }
				File.open("Model%i.txt" % i,'w') { |f| f.write(tmp) }
				@tmp_files.push("Model%i.txt" % i)
			end
			str=[]
			combinations.times do |i|
				str.push("\"Model%i.txt\"" % i)
			end
			res+=str.join(',')
		end
		res+="};\n"
		return res
	end

	def get_parameters()
		res="\nHammingPolyWeight\t"+@arguments['HammingPolyWeight'].to_s
		res+="\nComplexityWeight\t"+@arguments['ComplexityWeight'].to_s
		res+="\nRevEngWeight\t"+@arguments['RevEngWeight'].to_s
		res+="\nBioProbWeight\t"+@arguments['BioProbWeight'].to_s
		res+="\nHammingModelWeight\t"+@arguments['HammingModelWeight'].to_s
		res+="\nPolyScoreWeight\t"+@arguments['PolyScoreWeight'].to_s
		res+="\nGenePoolSize\t"+@arguments['GenePoolSize'].to_s
		res+="\nNumCandidates\t"+@arguments['NumCandidates'].to_s
		res+="\nNumParentsToPreserve\t"+@arguments['NumParentsToPreserve'].to_s
		res+="\nMaxGenerations\t"+@arguments['MaxGenerations'].to_s
		res+="\nStableGenerationLimit\t"+@arguments['StableGenerationLimit'].to_s
		res+="\nMutateProbability\t"+@arguments['MutateProbability'].to_s
		res+="\n"
		File.open("params.txt",'w') { |f| f.write(res) }
		@tmp_files.push("params.txt")
		return "PARAMS={\"params.txt\"};\n"
	end

	def create_fileManager(file_manager)
		out=""
		begin
			out+=self.get_fieldCardinality()
			out+=self.get_numberOfVariables()
			out+=self.get_wildType()
			out+=self.get_knockOut()
			out+=self.get_priorReverseEngineeringNetwork()
			out+=self.get_priorBiologicalNetwork()
			out+=self.get_priorModel()
			out+=self.get_parameters()
		rescue StandardError=>err
			STDERR.puts err
			return false
		end
		File.open(file_manager,'w') { |f| f.write(out) }
		@tmp_files.push(file_manager)
		@file_manager=file_manager
		return true
	end

	def render_output(raw_output_file)
		if !File.exists?(raw_output_file) then
			puts "RAW_OUTPUT_FILE_DOES_NOT_EXISTS"
			return
		end
		functions={}
		idx_func=1
		skip=false
		function=""
		File.foreach(raw_output_file) do |line|
			if line.strip().empty?
				idx_func=1
			elsif line.lstrip() =~ /^f.*$/
				function=line.split(" = ")[1]
				fx="f%i" % idx_func
				skip=false
			elsif line.lstrip() =~ /^H.*$/
				if !skip then
					tmp=line.split(' ')
					vars={
						'H'=>tmp[1].to_f,
						'C'=>tmp[3].to_f,
						'R'=>tmp[4].to_f,
						'B'=>tmp[7].to_f,
						'polynomialFunction'=>function.strip
					}
					fx="x%i" % idx_func
					if functions[fx].nil? then
						functions[fx]={function=>vars}
					elsif functions[fx][function].nil? then
						functions[fx][function]=vars
					end
					skip=true
					idx_func+=1
				else
					skip=false
				end
			end
		end
		variableScores_tmp={}
		functions.each do |f,j|
			variableScores_tmp[f]={}
			n=0
			j.each do |k,v|
				n+=1
				v['inputVariables']=k.scan(/x\d/).uniq
				v['score']=(v['H']+v['C']+v['R']+v['B'])/4
				v.delete('H')
				v.delete('C')
				v.delete('R')
				v.delete('B')
				v['inputVariables'].each do |v|
					if variableScores_tmp[f][v].nil? then
						variableScores_tmp[f][v]=1
					else
						variableScores_tmp[f][v]+=1
					end
				end
			end
			variableScores_tmp[f].each do |k,v|
				variableScores_tmp[f][k]=(v.to_f/n.to_f).to_f.round(2)
			end
		end
		updateRules=[]
		functions.each do |k,v|
			tmp=[]
			v.each { |i,j| tmp.push(j) }
			updateRules.push({"target"=>k,"functions"=>tmp})
		end
		variableScores=[]
		variableScores_tmp.each do |k,v|
			tmp=[]
			v.each do |i,j|
				tmp.push({"source"=>i,"score"=>j})
			end
			variableScores.push({"target"=>k,"sources"=>tmp})
		end
		@output={
			"type"=>"PolynomialDynamicalSystemSet",
			"description"=>"PLEASE FILL IN",
			"name"=>"reverseEngineeringOutputModel",
			"fieldCardinality"=>2,
			"variableScores"=>variableScores,
			#"variables"=>variableScores,
			"updateRules"=>updateRules
		}
		return @output
	end

	def run(output_file,params=nil)
		if output_file.nil? or output_file.empty? then
			puts "Error: no output file given"
			return 1
		end
		test_flag=''
		if params==:test then
			test_flag=' --test'
		end
		if self.create_fileManager(ENV["CODE_HOME"]+"/fileman.txt") then
			system(@exec_file+' '+@file_manager+' '+output_file+test_flag)
			if $?.exitstatus>0 or !File.exists?(output_file) then
				puts "Error: error occured while trying to run algorithm"
				return 1
			end
			return 0
		end
	end

	def test(output_ref)
		check1=output_ref['output']['variableScores']==@output['output']['variableScores']
		check2=output_ref['output']['updateRules']==@output['output']['updateRules']
		return (check1 and check2)
	end
end

if $0 == __FILE__ then
	input = File.read(ARGV[0])
	json = JSON.parse(input)
	task=React.new(json,'/home/algorithm/src/React')
	x = task.run("output.raw")
	File.open("output.txt",'w') { |f| f.write(JSON.pretty_generate(task.render_output("output.raw"))) }
end
