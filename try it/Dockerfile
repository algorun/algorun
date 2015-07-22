# Don't change the following line
FROM ahosny/algorun:0.9.2

MAINTAINER Your Name <your_email@domain.com>

# Don't forget to fill files in algorun_info folder and put your source code in src folder
# Don't change the following two lines
ADD ./algorun_info /home/algorithm/web/algorun_info/
ADD ./src /home/algorithm/src/

# Add any algorithm dependencies here
RUN apt-get install -y ruby2.0

# Add your command to execute an algorithm as environment variable EXEC_COMMAND
ENV EXEC_COMMAND "ruby test.rb"

# Add your command options (if any) as environment variable COMMAND_OPTIONS
# EXAMPLE: ENV COMMAND_OPTIONS "-c"

# If you specified -f as a command line option for the output file, ignore this step
# If your code outputs results to a hard-coded file name, specify it as environment variable OUTPUT_FILE_NAME
ENV OUTPUT_FILE_NAME "output.txt"

# If your algorithm uses some default configuration, pass them as environment variables
ENV PARAM_1 value1
ENV PARAM_2 value2
