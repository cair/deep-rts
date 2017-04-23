FROM nvidia/cuda:8.0-cudnn5-devel-ubuntu16.04
MAINTAINER Per-Arne Andersen

RUN apt-get update && apt-get install -y apt-utils openssh-server git xorg-dev libglu1-mesa-dev python3 python3-pip
RUN apt-get remove -y libcudnn5 libcudnn5-dev
RUN apt-get install -y libcudnn5 libcudnn5-dev
RUN pip3 install tensorflow-gpu

RUN mkdir /var/run/sshd
RUN echo 'root:root' | chpasswd
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config

# SSH login fix. Otherwise user is kicked off after login
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd

ENV NOTVISIBLE "in users profile"
RUN echo "export VISIBLE=now" >> /etc/profile

EXPOSE 22
CMD ["/usr/sbin/sshd", "-D"]

ADD . /data

# Get DeepRTS
RUN git clone https://github.com/UIA-CAIR/DeepRTS.git && cd DeepRTS/scripts/INSTALL && sh install.sh

# Compile and run DeepRTS
ENTRYPOINT cd /DeepRTS/ && mkdir build && cd build && cmake .. && make -j 8 && chmod +x ../scripts/paralell.sh && sh ../scripts/paralell.sh && bash

