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

const KEY = "/test_dir";

test(function () {
    zookeeper::setDebugLevel(1);
    $zk = new zookeeper(TEST_ZOOKEEPER_FULL_URL, TEST_ZOOKEEPER_TIMEOUT);
    if (!$zk->exists(KEY) && $zk->errCode === -101) // key 不存在
    {
        $zk->create(KEY, 'parent');
    }
    var_dump($zk->create(KEY."/a", "hello", ZOO_EPHEMERAL));
    var_dump($zk->create(KEY."/b", "world", ZOO_EPHEMERAL));
    var_dump($zk->getChildren(KEY));
});
?>
--EXPECT--
string(11) "/test_dir/a"
string(11) "/test_dir/b"
array(2) {
  [0]=>
  string(1) "a"
  [1]=>
  string(1) "b"
}
string(11) "/test_dir/a"
string(11) "/test_dir/b"
array(2) {
  [0]=>
  string(1) "a"
  [1]=>
  string(1) "b"
}
