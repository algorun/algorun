var fs = require('fs');
var exec = require("child_process").exec;

module.exports = {
  run: function (input_data, callback) {
    try {
        command = '';
        if (process.env.exec_command) {
            command = process.env.exec_command;
            command = command.trim();
        } else {
            callback("Are you sure you defined EXEC_COMMAND?");
        }
        
        process.chdir(process.env.CODE_HOME + '/src/');
        input_file = 'input.txt';
        fs.writeFileSync(input_file, input_data);
        
        command_options = '';
        if (process.env.command_options) {
            command_options = process.env.command_options;
            command_options = ' ' + command_options.trim();
        }
        output_file = '';
        if (process.env.output_file_name) {
            output_file = process.env.output_file_name;
            output_file = output_file.trim();
            algo_command = command + ' ' + input_file + command_options;
            exec(algo_command, function (err, stdout, stderr) {
                if (!err){
                    result = fs.readFileSync(output_file, 'utf8');
                    callback(result);
                } else {
                    callback('ERROR: Cannot run the algorithm! ' + stderr);
                }
            });
        } else {
            output_file = 'output.txt';
            algo_command = command + ' ' + input_file + command_options + ' -f ' + output_file;
            exec(algo_command, function (err, stdout, stderr) {
                if (!err){
                    result = fs.readFileSync(output_file, 'utf8');
                    callback(result);
                } else {
                    callback('ERROR: Cannot run the algorithm!');
                }
            });
        }
    }
    catch (err) {
        callback('Are you sure you included your algorithm code in src folder?');
    }
  }
};