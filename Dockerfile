FROM ubuntu:16.04
MAINTAINER Per-Arne Andersen

RUN apt-get update && apt-get install -y apt-utils python3 python3-pip git xvfb

RUN pip3 install git+https://github.com/UIA-CAIR/DeepRTS.git

RUN Xvfb :99 -ac &

ENV SDL_VIDEODRIVER dummy

RUN python3 -m pyDeepRTS
