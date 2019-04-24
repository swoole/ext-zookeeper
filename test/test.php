<?php
use swoole\zookeeper;

go(function () {
    zookeeper::setDebugLevel(1);
    $zk = new zookeeper("127.0.0.1:2181", 2.5);
    $zk->addAuth('digest', 'user0:passwd0');
    var_dump($zk->errCode);
    var_dump("create", $zk->create("/test222", 'xxxx'), $zk->errCode);
    var_dump("set", $zk->set("/test222", 'rango'), $zk->errCode);
    var_dump("get", $zk->get("/test222"), $zk->errCode);
    //var_dump("delete", $zk->delete("/test222"), $zk->errCode);
});
