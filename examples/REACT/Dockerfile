# Fill files in algorun_info folder and put your source code in src folder
# Don't change the following three lines
FROM algorun/algorun
ADD ./algorun_info /home/algorithm/web/algorun_info/
ADD ./src /home/algorithm/src/

# Install any algorithm dependencies here
RUN apt-get update && \
apt-get install -y build-essential ruby
RUN apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*
RUN ruby /home/algorithm/src/run.rb make
RUN rm -f src/*.o

# [Optional] Sign your image
MAINTAINER Thibauld Favre <tfavre@gmail.com>
