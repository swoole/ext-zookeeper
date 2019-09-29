<?php

use swoole\zookeeper;

//require __DIR__ . '/../inc/skipif.inc';
//require __DIR__ . '/../inc/bootstrap.php';

try{
    zookeeper::setDebugLevel(1);
    $zk = new zookeeper(TEST_ZOOKEEPER_FULL_URL, TEST_ZOOKEEPER_TIMEOUT);
    $zk->get(TEST_ZOOKEEPER_PERSISTENT_KEY);
}catch(Exception $e){
  var_dump($zk);
}
