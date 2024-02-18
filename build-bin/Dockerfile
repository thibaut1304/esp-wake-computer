FROM alpine:3.18

RUN apk update && apk add wget tar sed curl bash

WORKDIR /workspace
COPY script.sh /workspace/
RUN chmod +x /workspace/script.sh

VOLUME ["/workspace/bin"]

ENTRYPOINT ["/workspace/script.sh"]
