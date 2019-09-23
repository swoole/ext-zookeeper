#!/usr/bin/env bash

set -x -e

CURRENT=`pwd`

export  PHPX_VERSION="v0.1.1"
export  SWOOLE_VERSION="master"

export  PHPX_BIN_URL="https://github.com/swoole/phpx/releases/download/$PHPX_VERSION/phpx"
export  PHPX_SRC_URL="https://github.com/swoole/phpx/archive/$PHPX_VERSION.tar.gz"

export  SWOOLE_SRC_URL="https://github.com/swoole/swoole-src/archive/$SWOOLE_VERSION.tar.gz"

[[ -d $CURRENT/data ]] || mkdir -p ./data

[[ -f $CURRENT/data/phpx.php ]] || wget -O $CURRENT/data/phpx.php $PHPX_BIN_URL

[[ -f $CURRENT/data/phpx.tar.gz ]] || wget -O $CURRENT/data/phpx.tar.gz $PHPX_SRC_URL

[[ -f $CURRENT/data/swoole-src.tar.gz ]] || wget -O $CURRENT/data/swoole-src.tar.gz $SWOOLE_SRC_URL

circleci local execute -e LOCAL=1 -v $CURRENT/data:/tmp/data 
