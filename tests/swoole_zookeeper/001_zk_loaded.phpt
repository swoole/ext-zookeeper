--TEST--
startup: do nothing
--SKIPIF--
<?php 
require __DIR__ . '/../inc/skipif.inc';
?>
--FILE--
<?php
echo true;
?>
--EXPECT--
1