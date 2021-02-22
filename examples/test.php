<?php
use Swoole\ZooKeeper;

go(function () {
    ZooKeeper::setDebugLevel(1);
    $zk = new ZooKeeper("127.0.0.1:2181", 2.5);
    if (!$zk->exists('/test222'))
    {
        var_dump("create", $zk->create("/test222", 'xxxx'), $zk->errCode);
    }
    var_dump("set", $zk->set("/test222", 'rango'), $zk->errCode);
    var_dump("get", $zk->get("/test222"), $zk->errCode);
    var_dump("delete", $zk->delete("/test222"), $zk->errCode);
});
