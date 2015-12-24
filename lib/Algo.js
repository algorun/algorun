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
                        var result_stream = fs.createReadStream(output_file);
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
  }
};
