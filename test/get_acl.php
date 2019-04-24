<?php
use swoole\zookeeper;

go(function () {
    zookeeper::setDebugLevel(1);
    $zk = new zookeeper("127.0.0.1:2181", 2.5);
    var_dump("create", $zk->create("/test_acl", 'acl test'), $zk->errCode);
    var_dump("get_acl", $zk->getAcl("/test_acl"), $zk->errCode);
});