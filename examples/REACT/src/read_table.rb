require 'json'
require 'pp'
require 'zlib'
require 'base64'

mx=[]
File.open("w1.txt").each do |line| 
	if line.strip.length!=0
	puts line.strip.length
	mx.push(line.split(' '))
	end
end
# pp mx
mx_zip = Zlib::Deflate.deflate(Marshal.dump(mx))
mx_encoded = Base64.encode64 mx_zip
json={"payload"=>mx_encoded}
json_export= JSON.pretty_generate(json)
puts "--- export ---"
pp json_export
json_loaded = JSON.parse(json_export)
puts "--- import ---"
pp json_loaded
tmp= Marshal.load(Zlib::Inflate.inflate(Base64.decode64(json_loaded["payload"])))
# pp tmp

