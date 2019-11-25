FROM ubuntu:18.04
MAINTAINER Per-Arne Andersen

RUN apt-get update && apt-get install -y apt-utils python3 python3-pip git xvfb build-essential cmake

RUN git clone https://github.com/UIA-CAIR/DeepRTS.git drts --recurse-submodules
RUN pip3 install -e drts

RUN cat drts/coding/requirements.txt | xargs -n 1 pip3 install; exit 0
RUN cat drts/requirements.txt | xargs -n 1 pip3 install; exit 0

RUN Xvfb :99 -ac &

ENV SDL_VIDEODRIVER dummy

RUN python3 drts/coding/main.py



