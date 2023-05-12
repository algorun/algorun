var express = require('express');
var nocache = require('nocache');
var bodyParser = require('body-parser');
var multer = require('multer');
var fs = require('fs');
var path = require('path');
var util = require("util");
var algo_run = require(path.join(__dirname, "/lib/Algo"));
var strip_json = require(path.join(__dirname, "/lib/strip-json-comments"));

var app = express();
var manifest_exec = {};
var v2_0 = false;

app.use(nocache());

function setVersionEnvironment(manifest){
    process.env.manifest_version = manifest['manifest_version'];
    if(manifest.hasOwnProperty('algo_exec')){
        process.env.algo_exec = manifest['algo_exec'];
        manifest_exec["algo_exec"] = manifest['algo_exec'];
        manifest_exec["algo_parameters"] = manifest["algo_parameters"]
    }
    switch(manifest["manifest_version"]){
        case "1.0":        
            // version 1.0 doesn't define algo_input_stream. The default was file.
            process.env.algo_input_stream = "file";
            if(manifest.hasOwnProperty('algo_output_filename')){
                process.env.algo_output_stream = manifest['algo_output_filename'];
            }
            break;
        case "1.1":
            // version 1.1 deines algo_input_stream for more flexibility accepting input
            if(manifest.hasOwnProperty('algo_input_stream')){
                process.env.algo_input_stream = manifest['algo_input_stream'];
            }
            if(manifest.hasOwnProperty('algo_output_stream')){
                process.env.algo_output_stream = manifest['algo_output_stream'];
            }
            break;
        case "1.2":
            // version 1.2 defines output_type as a complement to input_type. For now, it is the same logic as 1.1 as AlgoRun containers doesn't use it; the website does. The case is included here for future development
            if(manifest.hasOwnProperty('algo_input_stream')){
                process.env.algo_input_stream = manifest['algo_input_stream'];
            }
            if(manifest.hasOwnProperty('algo_output_stream')){
                process.env.algo_output_stream = manifest['algo_output_stream'];
            }
            break;
        case "1.3":
            // version 1.2 defines output_type as a complement to input_type. For now, it is the same logic as 1.1 as AlgoRun containers doesn't use it; the website does. The case is included here for future development
            if(manifest.hasOwnProperty('algo_input_stream')){
                process.env.algo_input_stream = manifest['algo_input_stream'];
            }
            if(manifest.hasOwnProperty('algo_output_stream')){
                process.env.algo_output_stream = manifest['algo_output_stream'];
            }
            break;
        case "2.0":
        	v2_0 = true;
        	if(manifest.hasOwnProperty("algo_input")){
        		manifest_exec["algo_input"] = manifest["algo_input"]
        	}

        	if(manifest.hasOwnProperty("algo_output")){
        		manifest_exec["algo_output"] = manifest["algo_output"]
        	}
        	break;
        default:
            // let the default be the latest version
            v2_0 = true;
        	if(manifest.hasOwnProperty("algo_input")){
        		manifest_exec["algo_input"] = manifest["algo_input"]
        	}

        	if(manifest.hasOwnProperty("algo_output")){
        		manifest_exec["algo_output"] = manifest["algo_output"]
        	}
    }
    for (var key in manifest["algo_parameters"]) {
        if (manifest["algo_parameters"].hasOwnProperty(key)) {
            process.env[key] = manifest["algo_parameters"][key];
        }
    }
}

// configure environment variables
var manifestFilePath = path.join(__dirname, '/web/algorun_info/manifest.json');

fs.readFile(manifestFilePath, {encoding: 'utf-8'}, function(err,data){
    if (!err){
        strip_json.stripJsonComments(data, function (result){
            var manifest = JSON.parse(result);
            setVersionEnvironment(manifest);
        }); 
    } else{
        console.log(err);
    }
});

app.use(bodyParser.json({limit: '1000mb'}));
app.use(bodyParser.urlencoded({limit: '1000mb', extended: true}));
app.use(bodyParser.urlencoded({ extended: true })); // for parsing application/x-www-form-urlencoded
app.use(multer()); // for parsing multipart/form-data

var last_used = 'never';

app.get('/version', function (req, res) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "X-Requested-With");
    res.header("Content-Type", "application/json; charset=utf-8");
    res.status = 200;
    res.send({"api_version": process.env.manifest_version});
});

app.post("/v1/run", function(req, res){
	last_used = new Date();
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "X-Requested-With");
    res.status = 500;
    req.socket.setTimeout(0);
    res.socket.setTimeout(0); 
    if(!v2_0){
    	var data_input = req.body.input;
		var file_input = req.files.input;
		var hrstart = process.hrtime();
	    if (data_input){
	        algo_run.run(data_input, function (result_type, result_stream){
	            res.status = 200;
	            if(result_type === 'text'){
	                var hrend = process.hrtime(hrstart);
	                res.header("Run-Time", util.format("Computation Execution Time: %ss %sms", hrend[0], hrend[1]/1000000));
	                res.send(result_stream);
	            } else {
	                var hrend = process.hrtime(hrstart);
	                res.header("Run-Time", util.format("Computation Execution Time: %ss %sms", hrend[0], hrend[1]/1000000));
	                result_stream.pipe(res);   
	            }
	        });
	    } else if(file_input){
	        fs.readFile(req.files.input.path, function (err, data) {
	            var newPath = process.env.CODE_HOME + '/src/input.txt';
	            fs.writeFile(newPath, data, function (err) {
	                algo_run.run(false, function (result_type, result_stream){
	                    res.status = 200;
	                    if(result_type === 'text'){
	                        var hrend = process.hrtime(hrstart);
	                        res.header("Run-Time", util.format("Computation Execution Time: %ss %sms", hrend[0], hrend[1]/1000000));
	                        res.send(result_stream);
	                    } else {
	                        var hrend = process.hrtime(hrstart);
	                        res.header("Run-Time", util.format("Computation Execution Time: %ss %sms", hrend[0], hrend[1]/1000000));
	                        result_stream.pipe(res);   
	                    }
	                });
	            });
	        });
	    } else {
	        res.status = 200;
	        res.send('No input provided!');
	    }
    } else{
    	res.send("Oops, you requested the wrong URL! Try posting to /v2/run instead of /v1/run.")
    }
})

app.post('/v2/run', function (req, res) {
    last_used = new Date();
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "X-Requested-With");
    res.status = 500;
    req.socket.setTimeout(0);
    res.socket.setTimeout(0); 
    if(v2_0) {
  		var data_input = {}
  		algo_input = manifest_exec['algo_input']
  		for(i in algo_input){
            input = algo_input[i]
            if(input.src === 'direct'){
                data_input[input.name] = req.body[input.name]
            } else{
                if(req.files[input.name] !== undefined) {
                    var result = fs.readFileSync(req.files[input.name].path, 'ascii')
                    var path = process.env.CODE_HOME + '/src/' + input.name;
                    fs.writeFileSync(path, result);
                } else {
                    data_input[input.name] = req.body[input.name];
                }
            }
  		}

		var hrstart = process.hrtime();
	    if (data_input){
	        algo_run.runv2_0(data_input, manifest_exec, function (result_type, result_stream){
	            res.status = 200;
	            if(result_type === 'text'){
	                var hrend = process.hrtime(hrstart);
	                res.header("Run-Time", util.format("Computation Execution Time: %ss %sms", hrend[0], hrend[1]/1000000));
	                res.send(result_stream);
	            } else {
	                var hrend = process.hrtime(hrstart);
	                res.header("Run-Time", util.format("Computation Execution Time: %ss %sms", hrend[0], hrend[1]/1000000));
	                result_stream.pipe(res);   
	            }
	        }); 
	    } else {
	        res.status = 200;
	        res.send('No input provided!');
	    }

    }
    else {
    	res.send("Oops, you requested the wrong URL! Try posting to /v1/run instead of /v2/run.")
    }
});

app.post('/v1/config', function (req, res) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "X-Requested-With");
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

app.post('/v2/config', function (req, res) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "X-Requested-With");
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

app.get('/v1/manifest', function (req, res) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "X-Requested-With");
    res.header("Content-Type", "application/json; charset=utf-8");
    res.status = 200;
    res.sendFile(manifestFilePath);
});
app.get('/v1/status', function (req, res) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "X-Requested-With");
    res.header("Content-Type", "application/json; charset=utf-8");
    res.status = 200;
    res.send({'last_used': last_used});
});

app.get('/v2/manifest', function (req, res) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "X-Requested-With");
    res.header("Content-Type", "application/json; charset=utf-8");
    res.status = 200;
    res.sendFile(manifestFilePath);
});
app.get('/v2/status', function (req, res) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "X-Requested-With");
    res.header("Content-Type", "application/json; charset=utf-8");
    res.status = 200;
    res.send({'last_used': last_used});
});

app.use(express.static(__dirname + '/web'));

var server = app.listen(8765, function () {

  var host = server.address().address;
  var port = server.address().port;

  console.log('AlgoRun server is listening on port: %s. Check port mapping for the visible port number ..', port);

});

process.on('SIGINT', function() {
    console.log("Stopping AlgoRun server ..");
    process.exit();
});
