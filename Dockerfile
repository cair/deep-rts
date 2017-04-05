FROM nvidia/cuda
MAINTAINER Per-Arne Andersen

ADD . /data

RUN cat /etc/apt/sources.list
RUN apt-get update && apt-get install git -y
RUN git clone https://github.com/UIA-CAIR/DeepRTS.git && cd DeepRTS/scripts/INSTALL && sh install.sh


