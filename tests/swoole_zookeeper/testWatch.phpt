--TEST--
swoole_zookeeper: test watch2
--SKIPIF--
<?php 
require __DIR__ . '/../inc/skipif.inc';
?>
--FILE--
<?php
declare(strict_types=1);

require __DIR__ . '/../inc/bootstrap.php';

use swoole\zookeeper;

test(function() {
    zookeeper::setDebugLevel(1);
    $zk = new zookeeper(TEST_ZOOKEEPER_FULL_URL, TEST_ZOOKEEPER_TIMEOUT);
    if (!$zk->exists(TEST_ZOOKEEPER_WATCH_KEY)) {
        $zk->create(TEST_ZOOKEEPER_WATCH_KEY, 'swoole');
    }
    $zk->watch(TEST_ZOOKEEPER_WATCH_KEY);
    $watching = 2;
    $zk->setWatcher(function (zookeeper $zk, string $key) use (&$watching) {
        var_dump($key);
        if (--$watching) {
            $zk->watch($key);
            $zk->set(TEST_ZOOKEEPER_WATCH_KEY, (string) $watching);
        }
    });

    $zk->set(TEST_ZOOKEEPER_WATCH_KEY, 'a');

    $time = microtime(true);
    while ($watching) {
        $zk->wait();
        if (microtime(true) - $time >= 10) {
            exit('failed');
        }
    }
});
?>
--EXPECTF--
string(11) "/test_watch"
string(11) "/test_watch"
string(11) "/test_watch"
string(11) "/test_watch"
