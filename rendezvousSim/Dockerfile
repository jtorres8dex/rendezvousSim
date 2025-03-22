FROM ubuntu:latest

WORKDIR /rendezvousSim
COPY . rendezvousSim/

# install compilation tools
RUN apt-get update && apt-get install -y make cmake && rm -rf /var/lib/apt/lists/*
RUN apt update && apt install -y build-essential cmake 

# get cpp libraries
RUN apt install -y libyaml-cpp-dev 
RUN apt install -y libeigen3-dev

# python tools
RUN apt install -y python3 python3-pip python3-venv python3-dev

# venv
RUN python3 -m venv /opt/venv
ENV PATH="/opt/venv/bin:$PATH"

# install python packages inside the venv
RUN pip install --upgrade pip
RUN pip install matplotlib numpy
