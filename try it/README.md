# How to run your algorithm using algorun

Using algorun as a standard container enables you to turn your command-line application into a ready-to-use web application.
You wrap all your algorithm dependencies and code in only three steps.

## Try it!

This illustration assumes you have a minimal knowledge with [Docker](https://www.docker.com/).

### Step 1: Prepare your algorithm

- Your algorithm should accept the input as a file and produce the output to another file.

- Your algorithm should run as following:
`command input_file [FLAGS] [-f output_file]` where:
  * `command` (mandatory): is the command you use through terminal to run your algorithm
  * `input_file` (mandatory): is the input_file your algorithm read and process
  * `FLAGS` (optional): If you have different modes for running the algorithm, you specify them as flags
  * `-f output_file` (optional): If your command allow to specify a specific output file as a parameter, it should be in this format. If not, your algorithm should still write the output to a file (you will specify its name in the next step)

- Collect your algorithm source code in src folder. Even if your algorithm source files are in a hierarchy, gather them after all under src directory.

- Edit the files under algorun_info directory to give information about your algorithm. These information will appear in the web page that is created for you after building your algorithm container.

### Step 2: Edit the Dockerfile

In the Dockerfile, follow the steps to wrap your algorithm in a container. If you got stuck, you may refer to the full documentation at http://algorun.org/

### Step 3: Build, run and test your container

- In you working directory, use the following command to build your container:
`docker build -t <username>/<algorithm> .`

- You should see a message `"Succesfully built <container_id>"`.

- Use the following commmand to run your container:
`docker run -d -p 31331:8765 <username>/<algorithm>`

- Open your web browser and navigate to: http://localhost:31331 to test your algorithm

## Need further help?

[Visit Algorun!](http://algorun.org) for further assistance.
