FROM alpine:latest

ARG INSTALLDIR=/opt
RUN apk update && apk upgrade
RUN apk add openssl
ENV OPENSSL=${INSTALLDIR}/openssl

CMD ["/bin/sh"]
STOPSIGNAL SIGTERM