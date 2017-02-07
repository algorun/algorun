var fs = require('fs');
var exec = require("child_process").exec;
var execSync = require("child_process").execSync;
var uuid = require("uuid")
var sha256 = require("sha256");
var cache = {"hash": "",
            "stdout": ""
            }

module.exports = {
  run: function (input_data, callback) {
    try {
        command = '';
        if (process.env.algo_exec) {
            command = process.env.algo_exec;
            command = command.trim();
        } else {
            callback("text", "Are you sure you defined algo_exec?");
        }
    
        process.chdir(process.env.CODE_HOME + '/src/');

        var algo_command = '';
        if (process.env.algo_input_stream === 'file'){
            // algorithm reads file name as the first command line argument
            input_file = 'input.txt';
            if(input_data != false){
                fs.writeFileSync(input_file, input_data);
            }
            algo_command = command + ' ' + input_file;
            
        } else if(process.env.algo_input_stream === 'stdin'){
            // algorithm reads input data from stdin using <
            input_file = 'input.txt';
            if(input_data != false){
                fs.writeFileSync(input_file, input_data);
            }
            algo_command = command + ' < ' + input_file;
            
        } else {
            // algorithm reads input data directly as a parameter
            algo_command = command + ' ' + input_data;
        }
        
        if(process.env.algo_output_stream){
            output_file = process.env.algo_output_stream;
            output_file = output_file.trim();
            exec(algo_command, function (err, stdout, stderr) {
                if (!err){
                    
                    if(process.env.algo_output_stream === 'stdout') {
                        // algorithm produces output to the standard output stream
                        callback("text", {"output": stdout});
                    } else {
                        // algorithm produces output to a file
                        var result = fs.createReadStream(output_file, 'ascii');
                        callback("text", {"output": result});
                    }
                } else {
                    callback("text", "ERROR: Cannot run the algorithm!\n" + stderr);
                }
            });
        } else {
            callback("text", "Are you sure you defined algo_output_stream?");
        }
    }
    catch (err) {
        callback("text", "Are you sure you included your algorithm code in src folder?" + err);
    }
  },

  runv2_0: function (input_data, manifest, callback) {
    try {
        path = process.env.CODE_HOME + "/web/output_images/"
        var files = fs.readdirSync(path);
        for (var i in files){
            var name = files[i];
            fs.unlinkSync(path + name)
        }
        command = '';
        if (manifest['algo_exec']) {
            command = manifest['algo_exec'];
            command = command.trim();
        } else {
            callback("text", "Are you sure you defined algo_exec?");
        }
        if(command.indexOf("$") === -1){
            callback("text", "ERROR: Your command should take at least one parameter")
        }

        process.chdir(process.env.CODE_HOME + '/src/');
        
        var command_arr = command.split(" ");
        var algo_command = '';
        var hashString = "";

        for(str in command_arr){
            algo_input = "";
            algo_str = command_arr[str];
            if(algo_str[0] == "$"){
                input_name = algo_str.substring(1, algo_str.length);
                if(!manifest['algo_input'] || manifest['algo_input'].length === 0){
                    callback("test", "ERROR: please define input parameters");
                }
                for(i in manifest['algo_input']){
                    input = manifest['algo_input'][i];
                    if(input.name == input_name) {
                        algo_input = input;
                        break;
                    }
                }
                if(algo_input.src == 'file'){
                    data = input_data[input_name]
                    if(data === undefined){
                        callback("text", "ERROR: are your sure you defined proper parameters?")
                    }
                    fs.writeFileSync(input_name, data);
                    hashString += data
                    algo_command += " " + input_name;
                } else {
                    data = input_data[input_name]
                    if(data === undefined){
                        callback("text", "ERROR: are your sure you defined proper parameters?")
                    }
                    hashString += data
                    algo_command += " " + data;
                }
            } else {
                algo_command += " " + algo_str;
            }
        }

        algo_command = algo_command.trim();
        for (key in manifest["algo_parameters"]){
        	value = process.env[key]
        	hashString += key + "=" + value
        }
        algo_hash = sha256(hashString);
        caching = process.env.caching == "on"

        if(manifest['algo_output'] && manifest['algo_output'].length > 0){
            output_files = manifest['algo_output'];
            output_dict = {}
            error = false;
            if(!caching || cache.hash != algo_hash){
            	try{
            		var result = execSync(algo_command)
                    cache.stdout = result
                    cache.hash = algo_hash
            	}
            	catch(err){
                    error = err;
            	}
            	finally{
            		for(i in manifest["algo_input"]){
                		input = manifest["algo_input"][i]
                		try{
                			fs.unlinkSync(process.env.CODE_HOME + "/src/" + input.name)
                		}
                		catch(err){
                			console.log(input.name + " not deleted")
                			continue
                		}
                	}
            	}
            }
            if(!error){
                for(i in output_files){
                    output = output_files[i];
                    if(output.src === 'stdout'){
                        output_dict[output.name] = cache.stdout.toString();
                    } else {
                        type = output.type
                        if(type.split("/")[0] == "image"){
                            randVal = uuid.v1()
                            try{
                            	inStr = fs.createReadStream(process.env.CODE_HOME + '/src/' + output.src)
                            	outStr = fs.createWriteStream(process.env.CODE_HOME +"/web/output_images/" + randVal)
                            	inStr.pipe(outStr)
                            }
                            catch (err){
                            	callback("text", err)
                            }
                            output_dict[output.name] = "/output_images/" + randVal
                        } else {
                            var result = fs.readFileSync(output.src, 'ascii');
                            output_dict[output.name] = result;
                        }
                    }
                }
                callback('text', output_dict);
            } else {
                callback("text", "ERROR: Cannot run the algorithm!\n" + error);
            }
        } else {
            callback("text", "Are you sure you defined algo_output?");
        }
    }
    catch (err) {
        callback("text", "Are you sure you included your algorithm code in src folder? " + err);
    }
  }
};
