#    Algorun turns command-line algorithms into ready-to-use web enabled 
#    containers - Copyright (C) 2015 Thibauld Favre <tfavre@gmail.com>
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
FROM ubuntu:14.04
MAINTAINER Abdelrahman H. Ibrahim <abdelrahman.hosny@hotmail.com>
RUN apt-get update && \
apt-get install -y nodejs && \
apt-get clean && \
rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*
RUN apt-get update && \
apt-get install -y npm
RUN npm install express &&\
npm install body-parser &&\
npm install multer
ADD AsyncServer.js /home/algorithm/
ADD ./web/index.html /home/algorithm/web/
ADD ./web/js /home/algorithm/web/js/
ADD ./web/css /home/algorithm/web/css/
ADD ./web/algorun_info /home/algorithm/web/algorun_info/
ADD ./lib/*.js /home/algorithm/lib/
ENV CODE_HOME /home/algorithm/
EXPOSE 8765
ENTRYPOINT ["/usr/bin/nodejs","/home/algorithm/AsyncServer.js"]
