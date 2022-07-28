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

test(function () {
    zookeeper::setDebugLevel(1);
    $zk = new zookeeper(TEST_ZOOKEEPER_FULL_URL, TEST_ZOOKEEPER_TIMEOUT);
    if (!$zk->exists(TEST_ZOOKEEPER_PERSISTENT_KEY))
    {
        $zk->create(TEST_ZOOKEEPER_PERSISTENT_KEY, "swoole");
    }
    echo $zk->get(TEST_ZOOKEEPER_PERSISTENT_KEY), PHP_EOL;
});
?>
--EXPECTF--
swoole
swoole
