var express = require('express');
var bodyParser = require('body-parser');
var multer = require('multer');
var fs = require('fs');
var path = require('path');
var algo_run = require(path.join(__dirname, "/lib/Algo"));
var strip_json = require(path.join(__dirname, "/lib/strip-json-comments"));
var app = express();

// configure environment variables
var filePath = path.join(__dirname, '/web/algorun_info/manifest.json');
fs.readFile(filePath, {encoding: 'utf-8'}, function(err,data){
    if (!err){
        strip_json.stripJsonComments(data, function (result){
            var manifest = JSON.parse(result);
            for (var key in manifest["algo_parameters"]) {
                if (manifest["algo_parameters"].hasOwnProperty(key)) {
                    process.env[key] = manifest["algo_parameters"][key];
                }
            }
            if(manifest.hasOwnProperty('algo_exec')){
                process.env.algo_exec = manifest['algo_exec'];
            }
            if(manifest.hasOwnProperty('algo_output_filename')){
                process.env.algo_output_filename = manifest['algo_output_filename'];
            }
        }); 
    } else{
        console.log(err);
    }
});

app.use(bodyParser.json({limit: '100mb'}));
app.use(bodyParser.urlencoded({limit: '100mb', extended: true}));
app.use(bodyParser.urlencoded({ extended: true })); // for parsing application/x-www-form-urlencoded
app.use(multer()); // for parsing multipart/form-data

app.post('/run', function (req, res) {
    res.status = 500;
    req.socket.setTimeout(0);
    res.socket.setTimeout(0);
    var input = req.body.input;
    if (input){
        algo_run.run(input, function (result){
            res.status = 200;
            res.send(result);
        });        
    } else {
        res.status = 200;
        res.send('No input provided!');
    }
});
app.post('/config', function (req, res) {
    var env_var = req.body;
    if (env_var) {
        output = '';
        for (var key in env_var) {
            if (env_var.hasOwnProperty(key)) {
                if (process.env[key]) {
                    process.env[key] = env_var[key];
                    output += "Parameter " + key + " changed to " + env_var[key];
                } else {
                    output += "Cannot find " + key + " in environment variables";
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
app.post('/manifest', function (req, res) {
    res.status = 200;
    res.sendFile(filePath);
});

app.use(express.static(__dirname + '/web'));

var server = app.listen(8765, function () {

  var host = server.address().address;
  var port = server.address().port;

  console.log('Algorun server listening at http://%s:%s locally. Check port mapping for the visible port number ..', host, port);

});
