--TEST--
startup: do nothing
--SKIPIF--
<?php 
require __DIR__ . '/../inc/skipif.inc';
?>
--FILE--
<?php
declare(strict_types=1);

echo true;
?>
--EXPECT--
1