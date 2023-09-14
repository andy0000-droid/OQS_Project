ARG CURL_VERSION=7.87.0
ARG LIBOQS_BUILD_DEFINES="-DOQS_DIST_BUILD=ON"
ARG INSTALLDIR=/opt/oqskem

FROM ubuntu:22.04 as dev

ARG CURL_VERSION
ARG LIBOQS_BUILD_DEFINES
ARG INSTALLDIR

RUN apt install cmake 
RUN apt install gcc 
RUN apt install libtool
RUN apt install libssl-dev make ninja-build git -y

WORKDIR /opt
RUN git clone --depth 1 --branch oqs https://github.com/andy0000-droid/OQS_Project.git liboqs && \
    git clone --depth 1 --branch master https://github.com/openssl/openssl.git && \
    git clone --depth 1 --branch main https://github.com/open-quantum-safe/oqs-provider.git && \
    wget https://curl.haxx.se/download/curl-${CURL_VERSION}.tar.gz && tar -zxvf curl-${CURL_VERSION}.tar.gz;

WORKDIR /opt/liboqs
RUN mkdir build && cd build
RUN cmake -G"Ninja" .. && ninja

RUN LDFLAGS="-Wl,-rpath -Wl,${INSTALLDIR}/lib64" ./config shared --prefix=${INSTALLDIR} && \
    make ${MAKE_DEFINES} && make install_sw install_ssldirs;

CMD [ "sh" ]