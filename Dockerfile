# Use Ubuntu 16.04 LTS
# Multistage docker build, requires docker 17.05
FROM ubuntu:16.04 as builder
LABEL Author="Tiago Peralta tperalta82@gmail.com"

# If you have an old version of the docker, then
# correct the previous line, it should be the
# FROM fedora

RUN set -ex \
    && apt-get update \
    && apt-get dist-upgrade -y \
    && apt-get -y install build-essential cmake curl git time libsqlite3-dev screen \
    && apt-get clean

WORKDIR /app
RUN set -ex \
    && git clone https://github.com/openssl/openssl.git \
    && cd openssl \
    && ./Configure linux-x86_64 no-shared \
    && time make -j4

## Boost
ARG BOOST_VERSION=1_67_0
ARG BOOST_VERSION_DOT=1.67.0
ARG BOOST_HASH=2684c972994ee57fc5632e03bf044746f6eb45d4920c343937a465fd67a5adba
RUN set -ex \
    && curl -s -L -o  boost_${BOOST_VERSION}.tar.bz2 https://dl.bintray.com/boostorg/release/${BOOST_VERSION_DOT}/source/boost_${BOOST_VERSION}.tar.bz2 \
    && echo "${BOOST_HASH} boost_${BOOST_VERSION}.tar.bz2" | sha256sum -c \
    && tar -xvf boost_${BOOST_VERSION}.tar.bz2 \
    && mv boost_${BOOST_VERSION} boost \
    && cd boost \
    && ./bootstrap.sh \
    && ./b2 link=static -j 8 --build-dir=build64 --stagedir=stage

# LMDB
RUN set -ex \
    && git clone https://github.com/LMDB/lmdb.git

COPY . /app/zelerius

RUN set -ex \
    && mkdir /app/zelerius/build \
    && cd zelerius/build \
    && cmake .. \
    && time make -j4 \
    && cp -v ../bin/* /usr/local/bin \
    && mkdir /usr/local/bin/wallet_file \
    && cp -v ../tests/wallet_file/* /usr/local/bin/wallet_file \
    && apt-get --purge remove -y build-essential libssl-dev cmake curl git time libsqlite3-dev \
    && apt-get --purge autoremove -y \
    && apt-get clean all \
    && rm -rf /app \
    && echo '[ SHOW VERSION ]' \
    && zeleriusd -v

# If you have an old version of the docker:
# (not supported Multistage docker build)
# Please comment all the lines below this!

FROM ubuntu:16.04
LABEL Author="Tiago Peralta tperalta82@gmail.com"

COPY --from=builder /usr/local/bin/walletd /usr/local/bin/
COPY --from=builder /usr/local/bin/zeleriusd /usr/local/bin/
RUN ls -la /usr/local/bin/ \
    && zeleriusd -v

RUN mkdir /root/zelerius-wallet/

VOLUME [ "/root/.zelerius" ]
VOLUME [ "/root/zelerius-wallet" ]
EXPOSE  48080 48081 48082


