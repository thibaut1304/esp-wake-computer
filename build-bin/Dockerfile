FROM alpine:3.18


RUN apk update && \
apk add --no-cache wget tar sed curl bash ca-certificates && \
apk add --no-cache python3 py3-pip && \
apk add --no-cache gcc g++ make file

RUN wget -q -O /etc/apk/keys/sgerrand.rsa.pub https://alpine-pkgs.sgerrand.com/sgerrand.rsa.pub && \
	wget https://github.com/sgerrand/alpine-pkg-glibc/releases/download/2.33-r0/glibc-2.33-r0.apk && \
	apk add --no-cache glibc-2.33-r0.apk

RUN pip install pyserial

WORKDIR /workspace
COPY script/ /workspace/

RUN chmod +x /workspace/launch.sh
RUN chmod +x /workspace/parse-libraries.py
