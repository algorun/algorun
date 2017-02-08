# Fill files in algorun_info folder and put your source code in src folder
# Don't change the following three lines
FROM algorun/algorun
ADD ./algorun_info /home/algorithm/web/algorun_info/
ADD ./src /home/algorithm/src/

# Install any algorithm dependencies here
ENV PATH /home/algorithm/src/cufflinks:$PATH

# [Optional] Sign your image
LABEL "maintainer" <abdelrahman.hosny@hotmail.com>
