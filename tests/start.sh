#!/bin/sh
__CURRENT__=`pwd`
__DIR__=$(cd "$(dirname "$0")";pwd)

clear_php()
{
  ps -A | grep \.php$ | grep -v phpstorm | grep -v php-fpm | awk '{print $1}' | xargs kill -9 > /dev/null 2>&1
}

## before tests
clear_php
if [ `ulimit -n` -le 16384 ]; then
    ulimit -n 16384 > /dev/null 2>&1
fi
# run tests
if [ -z "${TEST_PHP_EXECUTABLE}" ]; then
    export TEST_PHP_EXECUTABLE=`which php`
fi

NO_INTERACTION=1 PHPT=1 ${TEST_PHP_EXECUTABLE} -d "memory_limit=1024m" ${__DIR__}/run-tests swoole_*
