FROM nvidia/cuda:8.0-runtime-ubuntu16.04
MAINTAINER Per-Arne Andersen

RUN apt-get update && apt-get install -y apt-utils
RUN apt-get update && apt-get install -y openssh-server
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

RUN apt-get update && apt-get install git -y
RUN apt-get update && apt-get install xorg-dev libglu1-mesa-dev -y
RUN git clone https://github.com/UIA-CAIR/DeepRTS.git && cd DeepRTS/scripts/INSTALL && sh install.sh

