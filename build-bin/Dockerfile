FROM alpine:3.18

RUN apk update && apk add --no-cache wget tar sed curl bash ca-certificates

WORKDIR /workspace
COPY script/launch.sh /workspace/
COPY required-libraries.txt /workspace/
COPY src/ /workspace/
RUN chmod +x /workspace/launch.sh

VOLUME ["/workspace/bin"]

ENTRYPOINT ["/workspace/launch.sh"]
