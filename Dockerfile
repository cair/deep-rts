FROM nvidia/cuda
MAINTAINER Per-Arne Andersen


RUN cat /etc/apt/sources.list
RUN apt-get update && apt-get install git -y
RUN git clone https://github.com/UIA-CAIR/DeepRTS.git && cd DeepRTS/scripts/INSTALL && sh install.sh


