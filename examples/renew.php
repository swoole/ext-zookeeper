<?php
use swoole\zookeeper;

go(function () {
    zookeeper::setDebugLevel(1);
    $zk = new zookeeper("127.0.0.1:2181", 10);
    var_dump($zk, "create", $zk->create("/test333", 'xxxx', ZOO_EPHEMERAL), $zk->errCode);
    while(true) {
        co::sleep(5);
        var_dump("exists", $zk->exists("/test333"), $zk->errCode);
    }
});