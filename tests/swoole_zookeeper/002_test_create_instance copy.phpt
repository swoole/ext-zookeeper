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
    var_dump($zk->getState()); // 未连接状态，返回 999
    var_dump($zk instanceof zookeeper);
    var_dump($zk->create(TEST_ZOOKEEPER_KEY, 1, ZOO_EPHEMERAL));
});
?>
--EXPECTF--
int(999)
bool(true)
string(21) "/test_create_instance"