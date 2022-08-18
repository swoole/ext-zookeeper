--TEST--
swoole_zookeeper: test getCore
--SKIPIF--
<?php
require __DIR__ . '/../inc/skipif.inc';
?>
--FILE--
<?php
declare(strict_types=1);

include_once __DIR__ . '/../inc/bootstrap.php';
use swoole\zookeeper;


test(function(){
    try{
        zookeeper::setDebugLevel(1);
        $zk = new zookeeper(TEST_ZOOKEEPER_FULL_URL, TEST_ZOOKEEPER_TIMEOUT);

        if (!$zk->exists(TEST_ZOOKEEPER_PERSISTENT_KEY))
        {
            $zk->create(TEST_ZOOKEEPER_PERSISTENT_KEY, "swoole");
        }

        $r = $zk->get(TEST_ZOOKEEPER_PERSISTENT_KEY);
        var_dump($r);
    }
    catch(Exception $e)
    {
      var_dump($zk);
    }
});
?>
--EXPECT--
string(6) "swoole"
string(6) "swoole"

