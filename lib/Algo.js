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
        input_file = 'input.txt';
        if(input_data != false){
            fs.writeFileSync(input_file, input_data);
        }

        if (process.env.algo_output_filename) {
            output_file = process.env.algo_output_filename;
            output_file = output_file.trim();
            algo_command = command + ' ' + input_file;
            exec(algo_command, function (err, stdout, stderr) {
                if (!err){
                    // to add support for algorithms that produce output to stdout
                    if(process.env.algo_output_filename === 'stdout') {
                        callback("text", stdout);
                    } else {
                        var result_stream = fs.createReadStream(output_file);
                        callback("stream", result_stream);
                    }
                } else {
                    callback("text", "ERROR: Cannot run the algorithm!\n" + stderr);
                }
            });
        } else {
            callback("text", "Are you sure you defined algo_output_filename?");
        }
    }
    catch (err) {
        callback("text", "Are you sure you included your algorithm code in src folder?");
    }
  }
};