--TEST--
swoole_zookeeper: test exists
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
    zookeeper::setDebugLevel(1);
    $zk = new zookeeper(TEST_ZOOKEEPER_FULL_URL, TEST_ZOOKEEPER_TIMEOUT);
    var_dump($zk->exists(TEST_ZOOKEEPER_INVALID_KEY), $zk->errCode);
    var_dump($zk->exists(TEST_ZOOKEEPER_NOT_EXISTS_KEY), $zk->errCode);
});
?>
--EXPECTF--
bool(false)
int(-8)
bool(false)
int(-101)
bool(false)
int(-8)
bool(false)
int(-101)