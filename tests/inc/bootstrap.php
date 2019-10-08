<?php

define('TEST_ZOOKEEPER_HOST', getenv('TEST_ZOOKEEPER_HOST') ?: '127.0.0.1');
define('TEST_ZOOKEEPER_PORT', getenv('TEST_ZOOKEEPER_PORT') ?: '2181');
define('TEST_ZOOKEEPER_TIMEOUT', getenv('TEST_ZOOKEEPER_TIMEOUT') ?: 1);
define("TEST_ZOOKEEPER_FULL_URL", TEST_ZOOKEEPER_HOST . ":" . TEST_ZOOKEEPER_PORT);
define('TEST_ZOOKEEPER_KEY', '/test_create_instance');
define('TEST_ZOOKEEPER_PERSISTENT_KEY', '/test_key_persistent');
define('TEST_ZOOKEEPER_INVALID_KEY', '/test_key_persistent/');
define('TEST_ZOOKEEPER_NOT_EXISTS_KEY', '/test_key_not_exists');