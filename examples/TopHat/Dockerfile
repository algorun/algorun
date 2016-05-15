# Fill files in algorun_info folder and put your source code in src folder
# Don't change the following three lines
FROM algorun/algorun
ADD ./algorun_info /home/algorithm/web/algorun_info/
ADD ./src /home/algorithm/src/

# Install any algorithm dependencies here
RUN apt-get update && \
cd /home/algorithm/src/samtools && \
./configure --without-curses && \
make && \
make install

ENV PATH /home/algorithm/src/bowtie2:$PATH
ENV PATH /home/algorithm/src/tophat:$PATH

# [Optional] Sign your image
MAINTAINER Abdelrahman Hosny <abdelrahman.hosny@hotmail.com>
