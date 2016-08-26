var fs = require('fs');
var exec = require("child_process").exec;

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
                        callback("text", stdout);
                    } else {
                        // algorithm produces output to a file
                        var result = fs.createReadStream(output_file);
                        callback("stream", result_stream);
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

  runv1_4: function (input_data, manifest, callback) {
    try {
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

        //process.chdir(process.env.CODE_HOME + '/src/');
        process.chdir('/home/nickjm6/Documents/algotest/')
        
        var command_arr = command.split(" ");
        var algo_command = '';
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
                    try {
                        data = fs.readFileSync(input_name, 'ascii')
                    } catch (err){
                        data = input_data[input_name]
                    }
                    if(data === undefined){
                        callback("text", "ERROR: are your sure you defined proper parameters?")
                    }
                    fs.writeFileSync(input_name, data);
                    algo_command += " " + input_name;
                } else {
                    data = input_data[input_name]
                    if(data === undefined){
                        callback("text", "ERROR: are your sure you defined proper parameters?")
                    }
                    algo_command += " " + data;
                }
            } else {
                algo_command += " " + algo_str;
            }
        }

        algo_command = algo_command.trim();

        if(manifest['algo_output'] && manifest['algo_output'].length > 0){
            output_files = manifest['algo_output'];
            output_dict = {}
            exec(algo_command, function (err, stdout, stderr) {
                if (!err){
                    for(i in output_files){
                        output = output_files[i];
                        if(output.src === 'stdout'){
                            output_dict['stdout'] = stdout;
                        } else {
                            var result = fs.readFileSync(output.src, 'ascii');
                            output_dict[output.src] = result;
                        }
                    }
                    callback('text', output_dict);
                } else {
                    callback("text", "ERROR: Cannot run the algorithm!\n" + stderr);
                }
            });
        } else {
            callback("text", "Are you sure you defined algo_output?");
        }
    }
    catch (err) {
        callback("text", "Are you sure you included your algorithm code in src folder? " + err);
    }
  }
};
