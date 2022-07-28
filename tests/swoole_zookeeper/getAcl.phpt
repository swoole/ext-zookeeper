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
    var_dump($zk->getAcl(TEST_ZOOKEEPER_PERSISTENT_KEY));
});
?>
--EXPECTF--
array(2) {
  [0]=>
  array(11) {
    ["czxid"]=>
    int(%d)
    ["mzxid"]=>
    int(%d)
    ["ctime"]=>
    int(%d)
    ["mtime"]=>
    int(%d)
    ["version"]=>
    int(0)
    ["cversion"]=>
    int(0)
    ["aversion"]=>
    int(0)
    ["ephemeralOwner"]=>
    int(0)
    ["dataLength"]=>
    int(6)
    ["numChildren"]=>
    int(0)
    ["pzxid"]=>
    int(%d)
  }
  [1]=>
  array(1) {
    [0]=>
    array(3) {
      ["perms"]=>
      int(31)
      ["scheme"]=>
      string(5) "world"
      ["id"]=>
      string(6) "anyone"
    }
  }
}
array(2) {
  [0]=>
  array(11) {
    ["czxid"]=>
    int(%d)
    ["mzxid"]=>
    int(%d)
    ["ctime"]=>
    int(%d)
    ["mtime"]=>
    int(%d)
    ["version"]=>
    int(0)
    ["cversion"]=>
    int(0)
    ["aversion"]=>
    int(0)
    ["ephemeralOwner"]=>
    int(0)
    ["dataLength"]=>
    int(6)
    ["numChildren"]=>
    int(0)
    ["pzxid"]=>
    int(%d)
  }
  [1]=>
  array(1) {
    [0]=>
    array(3) {
      ["perms"]=>
      int(31)
      ["scheme"]=>
      string(5) "world"
      ["id"]=>
      string(6) "anyone"
    }
  }
}

