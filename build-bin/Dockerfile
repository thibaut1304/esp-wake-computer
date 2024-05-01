FROM ubuntu:22.04

RUN apt-get update && \
	apt-get install -y wget tar sed curl bash ca-certificates vim && \
	apt-get install -y python3 python3-pip gcc g++ make file

RUN pip3 install pyserial

WORKDIR /workspace

COPY script/ /workspace/

RUN chmod +x /workspace/launch.sh
RUN chmod +x /workspace/parse-libraries.py
