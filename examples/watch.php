
<?php
use swoole\zookeeper;

go(function () {
    zookeeper::setDebugLevel(1);
    $zk = new zookeeper("127.0.0.1:2181", 5000);

    var_dump("watch", $zk->watch("/test"), $zk->errCode);

    $zk->setWatcher(function ($_zk, $key) {
        var_dump($key);
    });

    while (true) {
        $retval = $zk->wait();
        var_dump($retval);
    }
});