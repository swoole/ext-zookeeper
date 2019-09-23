
<?php
use swoole\zookeeper;

go(function () {
    zookeeper::setDebugLevel(1);
    $zk = new zookeeper("127.0.0.1:2181", 500);
    while(true) {
        var_dump("watch", $zk->watch("/test"), $zk->errCode);
        var_dump("-----------------------------------------------------", $zk->get('/test'));
        Co::sleep(2);
    }
});