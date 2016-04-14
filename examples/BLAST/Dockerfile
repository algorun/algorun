# Fill files in algorun_info folder and put your source code in src folder
# Don't change the following three lines
FROM algorun/algorun

# Install any algorithm dependencies here
RUN apt-get install -y wget
RUN wget ftp://ftp.ncbi.nlm.nih.gov/blast/executables/blast+/LATEST/ncbi-blast-2.3.0+-x64-linux.tar.gz && \
tar -xf ncbi-blast-2.3.0+-x64-linux.tar.gz

ADD ./algorun_info /home/algorithm/web/algorun_info/
ADD ./src /home/algorithm/src/

# [Optional] Sign your image
MAINTAINER Abdelrahman Hosny <abdelrahman.hosny@hotmail.com>
