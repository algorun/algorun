# Algorun

Algorun aims at defining a standard container to turn a command-line application into a ready-to-use web application.

## About algorun

### The problem: science reproducibility in computational sciences is broken

Science reproducibility is a big deal. If it is understandable that science reproducibility is hard to achieve in non-computational fields, it should be a no-brainer in all computational fields. Yet, it is most of the time still a non-trivial task to reproduce somebody else's results in computational sciences. We can do better in 2015.

### Algorun: providing a standard for science reproducibility in computational sciences

Algorun's goal is to enable scientists to turn any command-line application into a ready-to-use web application. Algorun relies heavily on Docker (http://github.com/docker). It basically wraps up and containerize a command line applications to make it easily distributable and usable through a nice web interface and a standard web API.

# How to run your algorithm using algorun

Using algorun as a standard container enables you to turn your command-line application into a ready-to-use web application.
You wrap all your algorithm dependencies and code in only three steps.

## Try it!

This illustration assumes you have a minimal knowledge with [Docker](https://www.docker.com/).

### Step 1: Prepare your algorithm

Prepare your algorithm to work in the following scenario:

- Accept the input as a file and produce the output to another file.

- Have a single entry point. That is, it starts execution from one point. If your algorithm start executing on multiple files, you should combine them such that the `algo_exec` (details below) operates on one file only.

- Read the first command line argument as the file name as following:
`command input_file` where:
  * `command` (mandatory): is the command you use through terminal to run your algorithm
  * `input_file` (mandatory): is the input_file your algorithm read and process

### Step 2: Wrap your algorithm in a container

To make the process easy, we have provided you with 'try it' directory that contains all what you need.

- First, you put all your algorithm source code in src folder.

- Second, edit the Dockerfile. Open the file and follow the instructions in it. Basically, you are going only to install algorithm dependencies.

- Finally, edit files in the algorun_info folder.

### Step 3: Build, run and test your container

- In you working directory, use the following command to build your container:
`docker build -t <username>/<algorithm> .`

- You should see a message `"Succesfully built <container_id>"`.

- Use the following commmand to run your container:
`docker run -d -p 31331:8765 <username>/<algorithm>`

- Open your web browser and navigate to: http://localhost:31331 to test your algorithm

## Need further help?

[Visit Algorun!](http://algorun.org) for further assistance.
