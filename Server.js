var express = require('express');
var bodyParser = require('body-parser');
var multer = require('multer');
var fs = require('fs');
var path = require('path');
//process.env.CODE_HOME = '/home/abdelrahman/uchc/algorun';
var algo_run = require(path.join(__dirname, "/lib/Algo"));
var app = express();

// configure environment variables
var filePath = path.join(__dirname, '/web/algorun_info/manifest.json');
fs.readFile(filePath, {encoding: 'utf-8'}, function(err,data){
    if (!err){
        var manifest = JSON.parse(data);
        for (var key in manifest["params"]) {
            if (manifest["params"].hasOwnProperty(key)) {
                process.env[key] = manifest["params"][key];
            }
        }
    }else{
        console.log(err);
    }
});

app.use(bodyParser.json()); // for parsing application/json
app.use(bodyParser.urlencoded({ extended: true })); // for parsing application/x-www-form-urlencoded
app.use(multer()); // for parsing multipart/form-data

app.post('/do/run', function (req, res) {
    res.status = 500;
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
app.post('/do/config', function (req, res) {
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

app.use(express.static(__dirname + '/web'));

var server = app.listen(8765, function () {

  var host = server.address().address;
  var port = server.address().port;

  console.log('Algorun server listening at http://%s:%s locally. Check port mapping for the visible port number ..', host, port);

});