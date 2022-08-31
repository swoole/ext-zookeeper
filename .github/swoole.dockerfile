ARG SWOOLE_VERSION
ARG PHP_VERSION

FROM phpswoole/swoole:${SWOOLE_VERSION}-php${PHP_VERSION}

RUN set -eux \
    && apt update && apt install -y cmake \
    && docker-php-ext-install -j$(nproc) mysqli
