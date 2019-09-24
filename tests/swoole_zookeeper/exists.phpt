--TEST--
swoole_zookeeper: test new instance
--SKIPIF--
<?php 
require __DIR__ . '/../inc/skipif.inc';
?>
--FILE--
<?php
require __DIR__ . '/../inc/bootstrap.php';

use swoole\zookeeper;

go(function () {
    zookeeper::setDebugLevel(1);
    $zk = new zookeeper(TEST_ZOOKEEPER_FULL_URL, TEST_ZOOKEEPER_TIMEOUT);
    var_dump($zk->exists(TEST_ZOOKEEPER_INVALID_KEY), $zk->errCode);
    var_dump($zk->exists(TEST_ZOOKEEPER_NOT_EXISTS_KEY), $zk->errCode);
});
Swoole\Event::wait();
?>
--EXPECTF--
bool(false)
int(-8)
bool(false)
int(-101)