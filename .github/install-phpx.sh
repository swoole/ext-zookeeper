#!/usr/bin/env bash
cd /tmp && \
curl -L -o phpx.tar.gz "https://github.com/Yurunsoft/phpx/archive/$PHPX_VERSION.tar.gz" && \
tar zxvf phpx.tar.gz -C ./ && cd ./phpx-$PHPX_VERSION && \
./build.sh && \
cp bin/phpx /usr/local/bin && \
cmake . && \
make -j && \
make install && \
ldconfig
