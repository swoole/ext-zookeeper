#!/usr/bin/env bash

set -x -e

CURRENT=`pwd`

export  PHPX_VERSION="0.1.1"
export  SWOOLE_VERSION="4.3.3"

export  PHPX_BIN_URL="https://github.com/swoole/phpx/releases/download/v$PHPX_VERSION/phpx"
export  PHPX_SRC_URL="https://github.com/swoole/phpx/archive/v$PHPX_VERSION.tar.gz"

export  SWOOLE_SRC_URL="https://github.com/swoole/swoole-src/archive/v$SWOOLE_VERSION.tar.gz"

[[ -d $CURRENT/data ]] || mkdir -p ./data

[[ -f $CURRENT/data/phpx.php ]] || wget -O $CURRENT/data/phpx.php $PHPX_BIN_URL

[[ -f $CURRENT/data/phpx.tar.gz ]] || wget -O $CURRENT/data/phpx.tar.gz $PHPX_SRC_URL

[[ -f $CURRENT/data/swoole-src.tar.gz ]] || wget -O $CURRENT/data/swoole-src.tar.gz $SWOOLE_SRC_URL

circleci local execute -e LOCAL=1 -v $CURRENT/data:/tmp/data 
