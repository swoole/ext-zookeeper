<?php
use swoole\zookeeper;

go(function () {
    zookeeper::setDebugLevel(1);
    $zk = new zookeeper("127.0.0.1:2181", 2.5);
    $a = $zk->getAcl("/test34");
    var_dump($a);
});