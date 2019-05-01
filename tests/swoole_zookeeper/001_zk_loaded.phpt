--TEST--
startup: do nothing
--SKIPIF--
<?php 
require __DIR__ . '/../inc/skipif.inc';
skip_if_no_zookeeper_server();
--FILE--
<?php
echo true;
--EXPECT--
1