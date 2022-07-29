#!/usr/bin/env bash
if [ $ZOOKEEPER_VERSION \> 3.4 ]; then
    export ZOO_SERVERS="server.1=zoo1:2888:3888;2181 server.2=zoo2:2888:3888;2181 server.3=zoo3:2888:3888;2181"
else
    export ZOO_SERVERS="server.1=zoo1:2888:3888 server.2=zoo2:2888:3888 server.3=zoo3:2888:3888"
fi
__DIR__=$(cd `dirname $0`; pwd)
cd $__DIR__
docker-compose up -d
