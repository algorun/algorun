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
            callback("Are you sure you defined algo_exec?");
        }
        
        process.chdir(process.env.CODE_HOME + '/src/');
        input_file = 'input.txt';
        fs.writeFileSync(input_file, input_data);
        
        if (process.env.algo_output_filename) {
            output_file = process.env.algo_output_filename;
            output_file = output_file.trim();
            algo_command = command + ' ' + input_file;
            exec(algo_command, function (err, stdout, stderr) {
                if (!err){
                    result = fs.readFileSync(output_file, 'utf8');
                    callback(result);
                } else {
                    callback('ERROR: Cannot run the algorithm! ' + stderr);
                }
            });
        } else {
            callback("Are you sure you defined algo_output_filename?");
        }
    }
    catch (err) {
        callback('Are you sure you included your algorithm code in src folder?');
    }
  }
};