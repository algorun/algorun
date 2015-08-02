# Fill files in algorun_info folder and put your source code in src folder
# Don't change the following three lines
FROM ahosny/algorun:0.9.3
ADD ./algorun_info /home/algorithm/web/algorun_info/
ADD ./src /home/algorithm/src/

# Install any algorithm dependencies here
RUN apt-get install -y ruby2.0

# [Optional] Sign your image
MAINTAINER Your Name <your_email@domain.com>
