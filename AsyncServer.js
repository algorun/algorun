var express = require('express');
var bodyParser = require('body-parser');
var multer = require('multer'); 
var app = express();
var exec = require("child_process").exec;

app.use(bodyParser.json()); // for parsing application/json
app.use(bodyParser.urlencoded({ extended: true })); // for parsing application/x-www-form-urlencoded
app.use(multer()); // for parsing multipart/form-data

app.post('/do/run', function (req, res) {
    res.status = 500;
    var input = req.body.input;
    if (input){
        var ruby_command = "ruby " + process.env.CODE_HOME + "lib/Algo.rb \'" + input + "\'";
        exec(ruby_command, function (err, stdout, stderr) {
            res.status = 200;
            if (err){
                res.send(stderr);
            } else {
                res.send(stdout);
            }
        });
        
    } else {
        res.status = 200;
        res.send('No input provided!');
    }
});
app.post('/do/config', function (req, res) {
    var env_var = req.body;
    if (env_var) {
        output = '';
        for (var key in env_var) {
            if (env_var.hasOwnProperty(key)) {
                if (process.env[key]) {
                    process.env[key] = env_var[key];
                    output += "Parameter " + key + " changed to " + env_var[key] + '<br>';
                } else {
                    output += "Cannot find " + key + " in environment variables<br>";
                }
            }
        }
        res.status = 200;
        res.send(output);
    } else {
        res.status = 200;
        res.send('No input provided!');
    }
});

app.use(express.static(__dirname + '/web'));

var server = app.listen(8765, function () {

  var host = server.address().address;
  var port = server.address().port;

  console.log('Algorun server listening at http://%s:%s', host, port);

});