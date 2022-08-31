--TEST--
swoole_zookeeper: test not connected
--SKIPIF--
<?php 
require __DIR__ . '/../inc/skipif.inc';
?>
--FILE--
<?php
declare(strict_types=1);

require __DIR__ . '/../inc/bootstrap.php';

use swoole\zookeeper;

test(function () {
    zookeeper::setDebugLevel(0);
    $zk = new zookeeper('127.0.0.1:80', TEST_ZOOKEEPER_TIMEOUT);
    var_dump($zk->getState()); // 未连接状态，返回 999
    var_dump($zk instanceof zookeeper);
    var_dump($zk->exists(TEST_ZOOKEEPER_KEY));
    var_dump($zk->set(TEST_ZOOKEEPER_KEY, 'swoole'));
    var_dump($zk->get(TEST_ZOOKEEPER_KEY));
});
?>
--EXPECTF--
int(999)
bool(true)
bool(false)
bool(false)
bool(false)
int(999)
bool(true)
bool(false)
bool(false)
bool(false)
